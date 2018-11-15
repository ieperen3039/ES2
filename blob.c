#include <stdlib.h>
#include <string.h>
#include "blob.h"
#include <stdio.h>
#include "logging.h"


typedef struct {
    int d;
    int h;
    int w;
} bin_blob_hdr;


//generic blob allocation which can be called with malloc or calloc as allocation function
BLOB* __alloc_blob(void*(*alloc)(size_t size), int d, int h, int w){
    BLOB* b=(BLOB*)(*alloc)(sizeof(BLOB));
    b->w=w;
    b->h=h;
    b->d=d;
    b->data=(float***) (*alloc)(sizeof(float**)*d);
    for(int z=0;z<d;z++){
        b->data[z]=(float**) (*alloc)(sizeof(float*)*h);
        for(int y=0;y<h;y++)
            b->data[z][y]= (float*) (*alloc)(sizeof(float)*w);
    }
    return b;
};

//custom calloc which matches the "malloc" signature
void* my_calloc(size_t size){
    return calloc(1, size);
}

//allocate blob with given dimensions
BLOB* alloc_blob(int d, int h, int w){
    return __alloc_blob(&malloc,d,h,w);
}

//calloc blob with given dimensions (zero init)
BLOB* calloc_blob(int d, int h, int w){
    return __alloc_blob(&my_calloc,d,h,w);
}

//free blob
void free_blob(BLOB* b){
    for(int z=0;z<b->d;z++){
        for(int y=0;y<b->h;y++)
            free(b->data[z][y]);
        free(b->data[z]);
    }
    free(b->data);
}

//duplicate blob
BLOB* duplicate_blob(BLOB* b){
    BLOB* out = alloc_blob(b->d, b->h, b->w);
    for(int z=0;z<out->d;z++)
        for(int y=0;y<out->h;y++)
            memcpy(out->data[z][y], b->data[z][y], sizeof(float)*b->w);
    return out;
}

//write blob to text file
void blob_write_txt(const char* fname, BLOB* b){

    //open file for writing
    FILE* fp = fopen(fname, "wt");
    if(fp==NULL)
        error("unable to open file %s for writing\n", fname);

    //write out dimensions
    fprintf(fp,"%d,%d,%d\n",b->d,b->h,b->w);

    //write out all data, one line per element
    for(int o=0;o<b->d;o++)
        for(int m=0;m<b->h;m++)
            for(int n=0;n<b->w;n++)
                fprintf(fp,"%f\n",b->data[o][m][n]);
    fclose(fp);
}

//write blob to binary file
void blob_write_bin(const char* fname, BLOB* b){

    //open file for writing
    FILE* fp = fopen(fname, "wb");
    if(fp==NULL)
        error("unable to open file %s for writing\n", fname);

    //create blob header
    bin_blob_hdr hdr;
    hdr.d=b->d;
    hdr.h=b->h;
    hdr.w=b->w;

    //write out header
    fwrite(&hdr, sizeof(bin_blob_hdr), 1, fp);

    //write out raw float data
    for(int o=0;o<b->d;o++)
        for(int m=0;m<b->h;m++)
            fwrite(b->data[o][m], sizeof(float), b->w, fp);


    //close the file
    fclose(fp);
}

//load blob from text file
BLOB* blob_read_txt(const char* fname){

    //open file for reading
    FILE* fp = fopen(fname, "rt");
    if(fp==NULL)
        error("unable to open file %s for reading\n", fname);

    //get dimensions
    int d,h,w;
    if(fscanf(fp,"%d,%d,%d\n",&d,&h,&w)!=3)
        error("parsing header from file %s\n", fname);

    //allocate BLOB
    BLOB* b = alloc_blob(d,h,w);

    //fill BLOB with data
    for(int o=0;o<b->d;o++)
        for(int m=0;m<b->h;m++)
            for(int n=0;n<b->w;n++)
                if(fscanf(fp,"%f\n",&(b->data[o][m][n]))!=1)
                    error("reading float value from %s\n", fname);

    //close the file
    fclose(fp);

    //return the parsed blob
    return b;
}

//load blob from binary file
BLOB* blob_read_bin(const char* fname){

    //open file for reading
    FILE* fp = fopen(fname, "rb");
    if(fp==NULL)
        error("unable to open file %s for reading\n", fname);

    //create blob header
    bin_blob_hdr hdr;

    //read out header
    if(fread(&hdr, sizeof(bin_blob_hdr), 1, fp)!=1)
        error("reading header from file %s\n", fname);

    //allocate blob
    BLOB* b = alloc_blob(hdr.d, hdr.h, hdr.w);

    //read out raw float data
    for(int o=0;o<b->d;o++)
        for(int m=0;m<b->h;m++)
            if((int)fread(b->data[o][m], sizeof(float), b->w, fp)!=b->w)
                error("reading float data from %s\n",fname);

    //close the file
    fclose(fp);

    //return the blob
    return b;
}
