#include "image_util.h"
#include "blob.h"
#include "convolution.h"
#include <stdlib.h>
#include "network.h"

BLOB* network(Network* net, IMG* img){

    //convert image to blob structure
    BLOB* input = (BLOB*) malloc(sizeof(BLOB));
    input->data = img->data;
    input->w    = img->width;
    input->h    = img->height;
    input->d    = img->channels;

    //perform convolution
    BLOB* out = convolution(input, &(net->layers[0].param.conv));

    return out;
}
