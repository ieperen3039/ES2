#include "image_util.h"
#include "blob.h"
#include "convolution.h"
#include <stdlib.h>


conv_param_t conv1 = {
    //number of output feature maps
    .num_out=32,

    //kernels sizes
    .Ky=3,
    .Kx=3,

    //strides
    .Sy=2,
    .Sx=2,

    //padding
    .pad=1,

    //weights
    .weights="bins/conv1_weights.bin",

    //bias
    .bias=NULL,

    //scaling
    .scale="bins/conv1_scale.bin",
    .scale_bias="bins/conv1_scale_bias.bin",

    //batch norm
    .bn_mean="bins/conv1_batchnorm_mean.bin",
    .bn_var="bins/conv1_batchnorm_variance.bin",
    .bn_eps=0.00001f,

    //relu
    .relu=true
};

BLOB* network(IMG* img){

    //convert image to blob structure
    BLOB* input = (BLOB*) malloc(sizeof(BLOB));
    input->data = img->data;
    input->w    = img->width;
    input->h    = img->height;
    input->d    = img->channels;

    //perform convolution
    BLOB* out = convolution(input, &conv1);

    return out;
}
