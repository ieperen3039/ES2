#ifndef NETWORK_H
#define NETWORK_H
#include "blob.h"
#include "image_util.h"
#include "convolution.h"

typedef union{
    conv_param_t conv;
} layer_param_t;

typedef enum{
    CONVOLUTION
}layer_type;

typedef struct{
    const char* name;
    layer_type type;
    layer_param_t param;
}layer_t;

typedef struct{
    const char* name;
    int num_layers;
    layer_t layers[];
}Network;

//inference on network
BLOB* network(Network* net, IMG* img);

#endif
