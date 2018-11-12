#ifndef MOBILE_NET_V2
#define MOBILE_NET_V2
#include "network.h"

Network mobilenetv2={
    .name="MobilenetV2",
    .num_layers=1,
    .layers={

        { //conv1
            .name="conv1",
            .type=CONVOLUTION,
            .param.conv={
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
           }
        }//end of conv1

    } //end of layers
};

#endif
