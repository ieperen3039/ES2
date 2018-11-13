#include "convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//add padding to blob
BLOB* pad(BLOB* in, int pad){

    //create output blob
    BLOB* out = calloc_blob(in->d, in->h+2*pad, in->w+pad*2);

    //copy non-padded input into output blob
    for(int z=0;z<in->d;z++)
       for(int y=0;y<in->h;y++)
          for(int x=0;x<in->w;x++)
              out->data[z][y+pad][x+pad]=in->data[z][y][x];

    //return pointer to padded blob
    return out;
}


BLOB* load_weights(BLOB* b, conv_param_t* p){

    //open weights file for reading
    FILE* fp = fopen(p->weights, "rb");
    if(fp==NULL){
        fprintf(stderr, "ERROR: could not open file %s for reading\n",p->weights);
        exit(-1);
    }

    //allocate 3D blob, and emulate 4D in KxKy later
    BLOB* w=alloc_blob(p->num_out, b->d, p->Ky*p->Kx);


    //fill 4D weight structure
    for(int o=0;o<p->num_out/p->group;o++)
        for(int i=0;i<b->d/p->group;i++)
            fread(w->data[o][i],sizeof(float),p->Ky*p->Kx, fp);

    //close file
    fclose(fp);

    //return weight blob
    return w;
}

float* load_1d(const char* fname, size_t num){

    //open file for reading
    FILE* fp = fopen(fname, "rb");
    if(fp==NULL){
        fprintf(stderr, "ERROR: could not open file %s for reading\n",fname);
        exit(-1);
    }

    //read in array
    float* arr= (float*) malloc(sizeof(float)*num);
    fread(arr,sizeof(float), num, fp);

    //close file
    fclose(fp);

    return arr;
}

//convolution, NOTE: destructive of BLOB* in. duplicate if further required!
BLOB* convolution(BLOB* input, conv_param_t* p){

    //use local pointer
    BLOB* in = input;

    //padding of input if required
    if(p->pad!=0)
        in = pad(in, p->pad);

    //create blob to hold output
    int height=(int)floor(((float)in->h - (float)p->Ky)/(float)p->Sy)+1;
    int width =(int)floor(((float)in->w - (float)p->Kx)/(float)p->Sx)+1;
    BLOB* out = alloc_blob(p->num_out, height, width);

    //load bias if required
    float* bias = NULL;
    if(p->bias!=NULL) bias=load_1d(p->bias, p->num_out);

    //set bias or init with zeroes
    for(int o=0;o<out->d;o++)
        for(int m=0;m<out->h;m++)
            for(int n=0;n<out->w;n++)
                out->data[o][m][n]=(bias!=NULL)?bias[o]:0.0f;

    //cleanup bias if initialized
    if(bias!=NULL) free(bias);


    //load weights
    BLOB* w = load_weights(in, p);

    //perform convolution
    for(int g=0;g<p->group;g++)
        for(int o=g*(out->d/p->group);o<(g+1)*(out->d/p->group);o++)
            for(int i=g*(in->d/p->group);i<(g+1)*(in->d/p->group);i++)
                for(int m=0;m<out->h;m++)
                    for(int n=0;n<out->w;n++)
                        for(int k=0;k<p->Ky;k++)
                            for(int l=0;l<p->Kx;l++)
                                out->data[o][m][n]+=in->data[i][m*p->Sy+k][n*p->Sx+l]*w->data[o][i][k*p->Kx+l];

    //free weights
    free_blob(w);

    //done with padded blob, free
    if(p->pad!=0)
        free_blob(in);

    //perform batchnorm if needed
    if(p->bn_mean!=NULL){
        //load batchnorm mean and variance
        float* mean = load_1d(p->bn_mean, out->d);
        float* var  = load_1d(p->bn_var, out->d);

        //batchnorm
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    out->data[o][m][n]= (out->data[o][m][n] - mean[o])/sqrtf(var[o]+p->bn_eps);

        //free mean and variance
        free(mean);
        free(var);
    }

    //perform scale if needed
    if(p->scale!=NULL){
        //load scale parameters
        float* scale = load_1d(p->scale, out->d);
        float* scale_bias = load_1d(p->scale_bias, out->d);

        //scale
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    out->data[o][m][n] = out->data[o][m][n]*scale[o] + scale_bias[o];

        //free parameters
        free(scale);
        free(scale_bias);
    }

    //perform relu
    if(p->relu==true){
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    out->data[o][m][n] =  fmax(0.0f, out->data[o][m][n]);
    }

    //return output
    return out;
}
