#include "eltwise.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//perform elementwise operation
BLOB* eltwise(BLOB** in_arr, eltwise_param_t* p){

    //copy first input layer
    BLOB* out=duplicate_blob(in_arr[0]);

    //loop over output elements
    for(int o=0;o<out->d;o++)
        for(int m=0;m<out->h;m++)
            for(int n=0;n<out->w;n++)
                //reduce all remaining blobs
                for(int i=1;in_arr[i]!=NULL;i++)
                    switch(p->type){

                        case SUM:
                            out->data[o][m][n]+=in_arr[i]->data[o][m][n];
                        break;

                        case PROD:
                            out->data[o][m][n]*=in_arr[i]->data[o][m][n];
                        break;

                        case MAX:
                            out->data[o][m][n]=fmax(out->data[o][m][n], in_arr[i]->data[o][m][n]);
                        break;
                    }

    //return output
    return out;
}


BLOB* eltwise2(BLOB* in1, BLOB* in2, eltwise_param_t* p){
    BLOB* list[3]={in1, in2, NULL};
    return eltwise(list, p);
}
