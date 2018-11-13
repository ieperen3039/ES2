#ifndef MOBILE_NET_V2
#define MOBILE_NET_V2
#include "network.h"

Network mobilenetv2={
    .name="MobilenetV2",
    .layers={


        //data (idx=0)
        //first layer is the input image
        //end data

        { //conv1 (idx=1)
            .name="conv1",
            .type=CONVOLUTION,
            .input=0,
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

                //group
                .group=1,

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
        },//end of conv1


        { //conv2_1_expand (idx=2)
            .name="conv2_1_expand",
            .type=CONVOLUTION,
            .input=1,
            .param.conv={
                //number of output feature maps
                .num_out=32,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv2_1_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_1_expand_scale.bin",
                .scale_bias="bins/conv2_1_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_1_expand_batchnorm_mean.bin",
                .bn_var="bins/conv2_1_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv2_1_expand

        { //conv2_1_dwise (idx=3)
            .name="conv2_1_dwise",
            .type=CONVOLUTION,
            .input=2,
            .param.conv={
                //number of output feature maps
                .num_out=32,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=32,

                //weights
                .weights="bins/conv2_1_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_1_dwise_scale.bin",
                .scale_bias="bins/conv2_1_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_1_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv2_1_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv2_1_dwise

        { //conv2_1_linear (idx=4)
            .name="conv2_1_linear",
            .type=CONVOLUTION,
            .input=3,
            .param.conv={
                //number of output feature maps
                .num_out=16,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv2_1_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_1_linear_scale.bin",
                .scale_bias="bins/conv2_1_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_1_linear_batchnorm_mean.bin",
                .bn_var="bins/conv2_1_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv2_1_linear


        { //conv2_2_expand (idx=5)
            .name="conv2_2_expand",
            .type=CONVOLUTION,
            .input=4,
            .param.conv={
                //number of output feature maps
                .num_out=96,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv2_2_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_2_expand_scale.bin",
                .scale_bias="bins/conv2_2_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_2_expand_batchnorm_mean.bin",
                .bn_var="bins/conv2_2_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv2_2_expand

        { //conv2_2_dwise (idx=6)
            .name="conv2_2_dwise",
            .type=CONVOLUTION,
            .input=5,
            .param.conv={
                //number of output feature maps
                .num_out=96,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=2,
                .Sx=2,

                //padding
                .pad=1,

                //group
                .group=96,

                //weights
                .weights="bins/conv2_2_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_2_dwise_scale.bin",
                .scale_bias="bins/conv2_2_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_2_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv2_2_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv2_2_dwise

        { //conv2_2_linear (idx=7)
            .name="conv2_2_linear",
            .type=CONVOLUTION,
            .input=6,
            .param.conv={
                //number of output feature maps
                .num_out=24,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv2_2_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv2_2_linear_scale.bin",
                .scale_bias="bins/conv2_2_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv2_2_linear_batchnorm_mean.bin",
                .bn_var="bins/conv2_2_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv2_2_linear


        { //conv3_1_expand (idx=8)
            .name="conv3_1_expand",
            .type=CONVOLUTION,
            .input=7,
            .param.conv={
                //number of output feature maps
                .num_out=144,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv3_1_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_1_expand_scale.bin",
                .scale_bias="bins/conv3_1_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_1_expand_batchnorm_mean.bin",
                .bn_var="bins/conv3_1_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv3_1_expand

        { //conv3_1_dwise (idx=9)
            .name="conv3_1_dwise",
            .type=CONVOLUTION,
            .input=8,
            .param.conv={
                //number of output feature maps
                .num_out=144,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=144,

                //weights
                .weights="bins/conv3_1_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_1_dwise_scale.bin",
                .scale_bias="bins/conv3_1_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_1_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv3_1_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv3_1_dwise

        { //conv3_1_linear (idx=10)
            .name="conv3_1_linear",
            .type=CONVOLUTION,
            .input=9,
            .param.conv={
                //number of output feature maps
                .num_out=24,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv3_1_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_1_linear_scale.bin",
                .scale_bias="bins/conv3_1_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_1_linear_batchnorm_mean.bin",
                .bn_var="bins/conv3_1_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv3_1_linear


        { //block3_1 (idx=11)
            .name="block3_1",
            .type=ELTWISE,
            .input=10,
            .input2=7,
            .param.eltwise={
                .type=SUM
           }
        },//end of block3_1

        { //conv3_2_expand (idx=12)
            .name="conv3_2_expand",
            .type=CONVOLUTION,
            .input=11,
            .param.conv={
                //number of output feature maps
                .num_out=144,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv3_2_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_2_expand_scale.bin",
                .scale_bias="bins/conv3_2_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_2_expand_batchnorm_mean.bin",
                .bn_var="bins/conv3_2_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv3_2_expand

        { //conv3_2_dwise (idx=13)
            .name="conv3_2_dwise",
            .type=CONVOLUTION,
            .input=12,
            .param.conv={
                //number of output feature maps
                .num_out=144,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=2,
                .Sx=2,

                //padding
                .pad=1,

                //group
                .group=144,

                //weights
                .weights="bins/conv3_2_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_2_dwise_scale.bin",
                .scale_bias="bins/conv3_2_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_2_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv3_2_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv3_2_dwise

        { //conv3_2_linear (idx=14)
            .name="conv3_2_linear",
            .type=CONVOLUTION,
            .input=13,
            .param.conv={
                //number of output feature maps
                .num_out=32,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv3_2_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv3_2_linear_scale.bin",
                .scale_bias="bins/conv3_2_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv3_2_linear_batchnorm_mean.bin",
                .bn_var="bins/conv3_2_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv3_2_linear


        { //conv4_1_expand (idx=15)
            .name="conv4_1_expand",
            .type=CONVOLUTION,
            .input=14,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_1_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_1_expand_scale.bin",
                .scale_bias="bins/conv4_1_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_1_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_1_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_1_expand

        { //conv4_1_dwise (idx=16)
            .name="conv4_1_dwise",
            .type=CONVOLUTION,
            .input=15,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=192,

                //weights
                .weights="bins/conv4_1_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_1_dwise_scale.bin",
                .scale_bias="bins/conv4_1_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_1_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_1_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_1_dwise

        { //conv4_1_linear (idx=17)
            .name="conv4_1_linear",
            .type=CONVOLUTION,
            .input=16,
            .param.conv={
                //number of output feature maps
                .num_out=32,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_1_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_1_linear_scale.bin",
                .scale_bias="bins/conv4_1_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_1_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_1_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_1_linear

        { //block4_1 (idx=18)
            .name="block4_1",
            .type=ELTWISE,
            .input=17,
            .input2=14,
            .param.eltwise={
                .type=SUM
           }
        },//end of block4_1

        { //conv4_2_expand (idx=19)
            .name="conv4_2_expand",
            .type=CONVOLUTION,
            .input=18,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_2_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_2_expand_scale.bin",
                .scale_bias="bins/conv4_2_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_2_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_2_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_2_expand

        { //conv4_2_dwise (idx=20)
            .name="conv4_2_dwise",
            .type=CONVOLUTION,
            .input=19,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=192,

                //weights
                .weights="bins/conv4_2_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_2_dwise_scale.bin",
                .scale_bias="bins/conv4_2_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_2_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_2_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_2_dwise

        { //conv4_2_linear (idx=21)
            .name="conv4_2_linear",
            .type=CONVOLUTION,
            .input=20,
            .param.conv={
                //number of output feature maps
                .num_out=32,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_2_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_2_linear_scale.bin",
                .scale_bias="bins/conv4_2_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_2_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_2_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_2_linear

        { //block4_2 (idx=22)
            .name="block4_2",
            .type=ELTWISE,
            .input=21,
            .input2=18,
            .param.eltwise={
                .type=SUM
           }
        },//end of block4_2


        { //conv4_3_expand (idx=23)
            .name="conv4_3_expand",
            .type=CONVOLUTION,
            .input=22,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_3_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_3_expand_scale.bin",
                .scale_bias="bins/conv4_3_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_3_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_3_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_3_expand

        { //conv4_3_dwise (idx=24)
            .name="conv4_3_dwise",
            .type=CONVOLUTION,
            .input=23,
            .param.conv={
                //number of output feature maps
                .num_out=192,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=192,

                //weights
                .weights="bins/conv4_3_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_3_dwise_scale.bin",
                .scale_bias="bins/conv4_3_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_3_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_3_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_3_dwise

        { //conv4_3_linear (idx=25)
            .name="conv4_3_linear",
            .type=CONVOLUTION,
            .input=24,
            .param.conv={
                //number of output feature maps
                .num_out=64,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_3_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_3_linear_scale.bin",
                .scale_bias="bins/conv4_3_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_3_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_3_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_3_linear


        { //conv4_4_expand (idx=26)
            .name="conv4_4_expand",
            .type=CONVOLUTION,
            .input=25,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_4_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_4_expand_scale.bin",
                .scale_bias="bins/conv4_4_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_4_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_4_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_4_expand

        { //conv4_4_dwise (idx=27)
            .name="conv4_4_dwise",
            .type=CONVOLUTION,
            .input=26,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=384,

                //weights
                .weights="bins/conv4_4_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_4_dwise_scale.bin",
                .scale_bias="bins/conv4_4_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_4_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_4_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_4_dwise

        { //conv4_4_linear (idx=28)
            .name="conv4_4_linear",
            .type=CONVOLUTION,
            .input=27,
            .param.conv={
                //number of output feature maps
                .num_out=64,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_4_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_4_linear_scale.bin",
                .scale_bias="bins/conv4_4_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_4_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_4_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_4_linear

        { //block4_4 (idx=29)
            .name="block4_4",
            .type=ELTWISE,
            .input=28,
            .input2=25,
            .param.eltwise={
                .type=SUM
           }
        },//end of block4_4


        { //conv4_5_expand (idx=30)
            .name="conv4_5_expand",
            .type=CONVOLUTION,
            .input=29,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_5_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_5_expand_scale.bin",
                .scale_bias="bins/conv4_5_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_5_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_5_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_5_expand

        { //conv4_5_dwise (idx=31)
            .name="conv4_5_dwise",
            .type=CONVOLUTION,
            .input=30,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=384,

                //weights
                .weights="bins/conv4_5_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_5_dwise_scale.bin",
                .scale_bias="bins/conv4_5_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_5_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_5_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_5_dwise

        { //conv4_5_linear (idx=32)
            .name="conv4_5_linear",
            .type=CONVOLUTION,
            .input=31,
            .param.conv={
                //number of output feature maps
                .num_out=64,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_5_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_5_linear_scale.bin",
                .scale_bias="bins/conv4_5_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_5_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_5_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_5_linear

        { //block4_5 (idx=33)
            .name="block4_5",
            .type=ELTWISE,
            .input=32,
            .input2=29,
            .param.eltwise={
                .type=SUM
           }
        },//end of block4_5


        { //conv4_6_expand (idx=34)
            .name="conv4_6_expand",
            .type=CONVOLUTION,
            .input=33,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_6_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_6_expand_scale.bin",
                .scale_bias="bins/conv4_6_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_6_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_6_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_6_expand

        { //conv4_6_dwise (idx=35)
            .name="conv4_6_dwise",
            .type=CONVOLUTION,
            .input=34,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=1,

                //group
                .group=384,

                //weights
                .weights="bins/conv4_6_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_6_dwise_scale.bin",
                .scale_bias="bins/conv4_6_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_6_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_6_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_6_dwise

        { //conv4_6_linear (idx=36)
            .name="conv4_6_linear",
            .type=CONVOLUTION,
            .input=35,
            .param.conv={
                //number of output feature maps
                .num_out=64,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_6_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_6_linear_scale.bin",
                .scale_bias="bins/conv4_6_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_6_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_6_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_6_linear

        { //block4_6 (idx=37)
            .name="block4_6",
            .type=ELTWISE,
            .input=36,
            .input2=33,
            .param.eltwise={
                .type=SUM
           }
        },//end of block4_6

        { //conv4_7_expand (idx=38)
            .name="conv4_7_expand",
            .type=CONVOLUTION,
            .input=37,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_7_expand_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_7_expand_scale.bin",
                .scale_bias="bins/conv4_7_expand_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_7_expand_batchnorm_mean.bin",
                .bn_var="bins/conv4_7_expand_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_7_expand

        { //conv4_7_dwise (idx=39)
            .name="conv4_7_dwise",
            .type=CONVOLUTION,
            .input=38,
            .param.conv={
                //number of output feature maps
                .num_out=384,

                //kernels sizes
                .Ky=3,
                .Kx=3,

                //strides
                .Sy=2,
                .Sx=2,

                //padding
                .pad=1,

                //group
                .group=384,

                //weights
                .weights="bins/conv4_7_dwise_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_7_dwise_scale.bin",
                .scale_bias="bins/conv4_7_dwise_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_7_dwise_batchnorm_mean.bin",
                .bn_var="bins/conv4_7_dwise_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=true
           }
        },//end of conv4_7_dwise

        { //conv4_7_linear (idx=40)
            .name="conv4_7_linear",
            .type=CONVOLUTION,
            .input=39,
            .param.conv={
                //number of output feature maps
                .num_out=96,

                //kernels sizes
                .Ky=1,
                .Kx=1,

                //strides
                .Sy=1,
                .Sx=1,

                //padding
                .pad=0,

                //group
                .group=1,

                //weights
                .weights="bins/conv4_7_linear_weights.bin",

                //bias
                .bias=NULL,

                //scaling
                .scale="bins/conv4_7_linear_scale.bin",
                .scale_bias="bins/conv4_7_linear_scale_bias.bin",

                //batch norm
                .bn_mean="bins/conv4_7_linear_batchnorm_mean.bin",
                .bn_var="bins/conv4_7_linear_batchnorm_variance.bin",
                .bn_eps=0.00001f,

                //relu
                .relu=false
           }
        },//end of conv4_7_linear



        {
            //last layer indicator
            .type=NONE
        }

    } //end of layers
};

#endif
