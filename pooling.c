#include "pooling.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//perform  pooling
BLOB* pooling(BLOB* in, pool_param_t* p){

    //for now this framework only supports global pooling
    if(!p->global){
        fprintf(stderr, "ERROR: only global pooling supported!\n");
        abort();
    }

    //global pooling, so output is 1x1
    BLOB* out = alloc_blob(in->d, 1, 1);

    //set bias or init with zeroes
    for(int o=0;o<out->d;o++){
        //init reduction result
        float r=(p->type==POOL_MAX)?in->data[o][0][0]:0.0f;
        for(int m=0;m<in->h;m++)
            for(int n=0;n<in->w;n++)
                switch(p->type){
                    case POOL_AVG:
                       r+=in->data[o][m][n];
                    break;
                    case POOL_MAX:
                       r=fmax(r,in->data[o][m][n]);
                    break;
                 }

        //divide by N for averaging
        if(p->type==POOL_AVG)
            r/=(in->h*in->w);

        //store result in output blob
        out->data[o][0][0]=r;
    }

    //return pooled result
    return out;
}
