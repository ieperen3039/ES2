#ifndef NETWORK_H
#define NETWORK_H
#include "blob.h"
#include "image_util.h"
#include "convolution.h"
#include "eltwise.h"

typedef union{
    conv_param_t conv;
    eltwise_param_t eltwise;
} layer_param_t;

typedef enum{
    CONVOLUTION,
    ELTWISE,
    NONE        //not a real layer, used to indicated last layer in list
}layer_type;

typedef struct{
    const char* name;
    layer_type type;
    layer_param_t param;
    int input;
    int input2;
}layer_t;

typedef struct{
    const char* name;
    layer_t layers[];
}Network;

//inference on network
BLOB* network(Network* net, IMG* img);

#endif
