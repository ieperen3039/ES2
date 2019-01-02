

#include "convolution_kernel.h"

/**
 * Runs the processing of a single pixel (kernel) on a GPU thread. Calculates dot product by using kernels.
 * @param output_blob pointer to the output blob, initialized with either zero or a bias.
 * @param input_blob pointer to input blob, including padding
 * @param conv_param_t the parameters of this convolution layer
 */
__kernel gpu_device_convolution(__global float* output_blob, __global float* input_blob, __global float* weights,
                                __global int sx, __global int sy, __global int kernelXSize, __global int kernelYSize) {

    // Location in the slice
    unsigned int x = get_global_id(0);
    unsigned int y = get_global_id(1);

    // location in the group, input/output position
    unsigned int z = get_global_id(2);

    unsigned int weightBase = weightRow * blob_width;

    printf("global location: (%d, %d, %d)\n", x, y, z);

    unsigned int blob_width = get_global_size(0); //gridDim.x*blockDim.x;
    unsigned int blob_height = get_global_size(1); //gridDim.y*blockDim.y;

    unsigned int targetIndex = z * blob_height * blob_width + y * blob_width + x;
    unsigned int accumulator = output_blob[targetIndex];

    for (int k = 0; k < kernelYSize; k++) {
        for (int l = 0; l < kernelXSize; l++) {
            unsigned int ky = y * sy + k;
            unsigned int lx = x * sx + l;
            unsigned int wx = k * kernelXSize + l;

            unsigned int valueInd = z * blob_height * blob_width + ky * blob_width + lx;
            unsigned int weightInd = z * blob_height * blob_width + weightBase + wx;

            accumulator += input_blob[valueInd] * weights[weightInd];
        }
    }

    output_blob[targetIndex] = accumulator;
}
