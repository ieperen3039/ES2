#include "network.h"
#include <stdbool.h>
#include <stdlib.h>

BLOB* network(Network* net, IMG* img){

    //convert image to blob structure
    BLOB* input = (BLOB*) malloc(sizeof(BLOB));
    input->data = img->data;
    input->w    = img->width;
    input->h    = img->height;
    input->d    = img->channels;

    //get number of layers in the network
    int num_layers=0;
    while(net->layers[num_layers].type!=NONE) num_layers++;

    //create placeholder for all layer blobs
    BLOB** layer_blobs=(BLOB**)malloc(sizeof(BLOB*)*num_layers+1);

    //insert input layer
    layer_blobs[0]=input;

    //iterate through the layers in the network
    bool done=false;
    for(int l=0;!done;l++){

        //select layer "l" from the network
        layer_t layer=net->layers[l];

        //depending on type the actions differ
        switch(layer.type){
            case CONVOLUTION:
                printf("Performing convolution of layer %s\n", layer.name);
                layer_blobs[l+1] = convolution(
                    layer_blobs[layer.input],
                    &(layer.param.conv)
                );
            break;

            case ELTWISE:
                printf("Performing elementwise operation of layer %s\n", layer.name);
                layer_blobs[l+1] = eltwise2(
                    layer_blobs[layer.input],
                    layer_blobs[layer.input2],
                    &(layer.param.eltwise)
                );

            break;

            case NONE:
                //last layer in the list
                done=true;
            break;
        }
    }

    return layer_blobs[num_layers];
}
