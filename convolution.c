#include "convolution_kernel.h"
#include "cl_kernels.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.h>
#include <cublas_v2.h>
#include <curand_kernel.h>

#define DO_GPU_CONVOLUTION

void setKernelArg(unsigned int argID, const cl_struct* p_kernel_env, cl_mem* data);

//add padding to blob
BLOB* pad(BLOB* in, int pad) {

    //create output blob
    BLOB* out = blob_calloc(in->d, in->h + 2 * pad, in->w + pad * 2);

    //copy non-padded input into output blob
    for (int z = 0; z < in->d; z++) {
        for (int y = 0; y < in->h; y++) {
            for (int x = 0; x < in->w; x++) {
                blob_data(out, z, y + pad, x + pad) = blob_data(in, z, y, x);
            }
        }
    }

    //return pointer to padded blob
    return out;
}

BLOB* load_weights(BLOB* b, conv_param_t* p) {

    //open weights file for reading
    FILE* fp = fopen(p->weights, "rb");
    if (fp == NULL) {
        printf("ERROR could not open file %s for reading\n", p->weights);
        exit(1);
    }

    //for fully connected layers the kernel size is equal to the input size
    int Ky = (p->fc) ? b->h : p->Ky;
    int Kx = (p->fc) ? b->w : p->Kx;

    //allocate 3D blob, and emulate 4D in KxKy later
    BLOB* w = blob_alloc(p->num_out, b->d / p->group, Ky * Kx);

    //fill 4D weight structure
    for (int g = 0; g < p->group; g++) {

        int firstIndex = g * (p->num_out / p->group);
        int lastIndex = firstIndex + g;
        for (int o = firstIndex; o < lastIndex; o++) {

            int firstIndex2 = g * (b->d / p->group);
            int lastIndex2 = firstIndex2 + g;
            for (int i = firstIndex2; i < lastIndex2; i++) {
                /* note: each output map has only  b->d/p->group input maps.
                 * Hence the absolute index of i is subtracted when storing in w */
                float data = blob_data(w, o, i - firstIndex2, 0);
                size_t fileSize = fread(&data, sizeof(float), Ky * Kx, fp);
                if ((int) fileSize != Ky * Kx) {
                    printf("ERROR: loading weights from file %s\n", p->weights);
                    exit(1);
                }
            }
        }
    }

    //close file
    fclose(fp);

    //return weight blob
    return w;
}

float* load_1d(const char* fname, size_t num) {

    //open file for reading
    FILE* fp = fopen(fname, "rb");
    if (fp == NULL) {
        printf("ERROR: could not open file %s for reading\n", fname);
        exit(1);
    }

    //read in array
    float* arr = (float*) malloc(sizeof(float) * num);
    if (fread(arr, sizeof(float), num, fp) != num) {
        printf("ERROR: loading data from file %s\n", fname);
        exit(1);
    }

    //close file
    fclose(fp);

    return arr;
}

void
gpu_kernel(const BLOB* in, const conv_param_t* p, int kernelYSize, int kernelXSize, const BLOB* out, const BLOB* w) {
    int err = CL_SUCCESS;
    printf("Starting GPU kernel\n");

    /* Initialise OpenCL kernel */
    // Note: this is only possible because the kernel environment is on the heap

    cl_struct* p_kernel_env = init_device((char*) "convolution_kernel.cl", (char*) "gpu_device_convolution");
    printf("Initializing arguments...\n");

    /* Copy image data from the CPU to the GPUs global memory */
    size_t blob_size = blob_bytes(in);

    // Input
    cl_mem input_blob = clCreateBuffer(p_kernel_env->context,
                                       CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                       blob_size,
                                       in->data,
                                       &err);
    CHECK_ERR(err);

    // Output
    cl_mem output_blob = clCreateBuffer(p_kernel_env->context,
                                        CL_MEM_WRITE_ONLY,
                                        blob_size,
                                        out->data,
                                        &err);
    CHECK_ERR(err);

    // p_sx
    int sx_val = p->Sx; // copy
    cl_mem sx = clCreateBuffer(p_kernel_env->context,
                               CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                               sizeof(int),
                               &sx_val,
                               &err);
    CHECK_ERR(err);

    // p_sy
    int sy_val = p->Sy; // copy
    cl_mem sy = clCreateBuffer(p_kernel_env->context,
                               CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                               sizeof(int),
                               &sy_val,
                               &err);
    CHECK_ERR(err);

    // kernelXSize
    cl_mem kx_size = clCreateBuffer(p_kernel_env->context,
                                    CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                    sizeof(int),
                                    &kernelXSize,
                                    &err);
    CHECK_ERR(err);

    // kernelYSize
    cl_mem ky_size = clCreateBuffer(p_kernel_env->context,
                                    CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                    sizeof(int),
                                    &kernelYSize,
                                    &err);
    CHECK_ERR(err);

    // weights
    cl_mem weight_matrix = clCreateBuffer(p_kernel_env->context,
                                          CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                          blob_size,
                                          (void*) w,
                                          &err);
    CHECK_ERR(err);
    printf("Loading arguments...\n");

    // Set buffers as kernel arguments
    setKernelArg(0, p_kernel_env, &output_blob);
    setKernelArg(1, p_kernel_env, &input_blob);
    setKernelArg(2, p_kernel_env, &weight_matrix);
    setKernelArg(3, p_kernel_env, &sx);
    setKernelArg(4, p_kernel_env, &sy);
    setKernelArg(5, p_kernel_env, &kx_size);
    setKernelArg(6, p_kernel_env, &ky_size);

    /* Perform the convolution on the GPU */

    //TODO the values for global_work_size and local_work_size are probably wrong
    size_t work_dim = 3;
    size_t global_work_size[work_dim]; // The total number of global work items is the product of all elements
    global_work_size[0] = (size_t) p->Sx;
    global_work_size[1] = (size_t) p->Sy;
    global_work_size[2] = (size_t) p->group;

    printf("Executing convolution on GPU...\n");

    err = clEnqueueNDRangeKernel(p_kernel_env->commands,    // Command queue
                                 p_kernel_env->kernel,      // Kernel code
                                 work_dim,
                                 NULL,                      // global_work_offset, must be NULL according to documentation
                                 global_work_size,
                                 NULL,
                                 0, NULL,
                                 NULL);            // There are no events we need to wait for, before we can start

    if (err != CL_SUCCESS) {
        printf("Error: Failed to execute kernel! %d\n", err);
        exit(EXIT_FAILURE);
    }

    /* Read output */
    printf("Reading output...\n");

    // Load result from __local memory
    BLOB result_blob;
    err = clEnqueueReadBuffer(p_kernel_env->commands,   // Command queue
                              output_blob,              // Pointer to output arg from clCreateBuffer(), TODO
                              CL_TRUE,                  // Blocking read
                              0,                        // Read offset
                              blob_size,                // Number of bytes to read
                              &result_blob,             // Buffer to read data into
                              0, NULL, NULL);           // No events

    if (err != CL_SUCCESS) {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    printf("GPU Kernel finished...\n");

    /*
     TODO:
     read result back into data
     */

    /* Release memory */
    close_device(p_kernel_env);

}

void setKernelArg(unsigned int argID, const cl_struct* p_kernel_env, cl_mem* data) {
    int err = clSetKernelArg(p_kernel_env->kernel, argID, sizeof(cl_mem), (void*) data); // Output pointer

    if (err != CL_SUCCESS) {
        printf("Error: Failed to set kernel argument %d: Error %d\n", argID, err);
        exit(1);
    }
}

void
cpu_kernel(const BLOB* in, const conv_param_t* p, int kernelYSize, int kernelXSize, const BLOB* out, const BLOB* w) {

    printf("Starting CPU kernel...\n");

    //perform convolution
    for (int g = 0; g < p->group; g++) {
        /* G:  Iterate over the number of groups (whatever a group is) */
        int firstOutputSlice = g * (out->d / p->group);
        int lastOutputSlice = firstOutputSlice + g;
        for (int o = firstOutputSlice; o < lastOutputSlice; o++) {
            /* O: Iterate over the output 'slices' (in the depth direction) within group G */
            int firstInputSlice = g * (in->d / p->group);
            int lastInputSlice = firstInputSlice + g;
            for (int i = firstInputSlice; i < lastInputSlice; i++) {
                /* I: Iterate over the input 'slices' (in the depth direction) within group G */

                for (int m = 0; m < out->h; m++) {
                    /* M: Iterate over the rows (height) of slice O */
                    for (int n = 0; n < out->w; n++) {
                        /* N: Iterate over the cells in row M */
                        // Calculate dot product by using kernels (for us 1x1 or 3x3)
                        for (int k = 0; k < kernelYSize; k++) {
                            for (int l = 0; l < kernelXSize; l++) {
                                /* note: absolute starting i is subtracted for
                                 the weights, see load_weights function for more info */
                                int ypos = m * (p->Sy) + k;
                                int xpos = n * (p->Sx) + l;
                                int wx = k * kernelXSize + l;
                                int wy = i - firstInputSlice;
                                blob_data(out, o, m, n) += blob_data(in, i, ypos, xpos) *
                                                           blob_data(w, o, wy, wx);
                            }
                        }
                    }
                }
            }
        }
    }

    printf("CPU Kernel finished...\n");
}

//convolution, NOTE: destructive of BLOB* in. duplicate if further required!
BLOB* convolution(BLOB* input, conv_param_t* p) {

    //use local pointer
    BLOB* in = input;

    //padding of input if required
    if (p->pad != 0)
        in = pad(in, p->pad);

    //if fully connected, the kernel size is set to the image size
    int kernelYSize = (p->fc) ? in->h : p->Ky;
    int kernelXSize = (p->fc) ? in->w : p->Kx;

    //create blob to hold output
    int height = (int) floorf(((float) in->h - (float) kernelYSize) / (float) p->Sy) + 1;
    int width = (int) floorf(((float) in->w - (float) kernelXSize) / (float) p->Sx) + 1;
    BLOB* out;

    //load bias if required
    if (p->bias == NULL) {
        //zero init
        out = blob_calloc(p->num_out, height, width);
    } else {

        printf("Loading bias...\n");
        //not required to calloc
        out = blob_alloc(p->num_out, height, width);

        //load bias values from file
        float* bias = load_1d(p->bias, p->num_out);

        //set bias or init with zeroes
        for (int o = 0; o < out->d; o++) {
            for (int m = 0; m < out->h; m++) {
                for (int n = 0; n < out->w; n++) {
                    blob_data(out, o, m, n) = bias[o];
                }
            }
        }

        //cleanup bias
        free(bias);
    }
    printf("Loading weights...\n");

    //load weights
    BLOB* w = load_weights(in, p);

#ifdef DO_GPU_CONVOLUTION
    gpu_kernel(in, p, kernelYSize, kernelXSize, out, w);
#else
    cpu_kernel(in, p, kernelYSize, kernelXSize, out, w);
#endif

    //free weights
    blob_free(w);

    //done with padded blob, free
    if (p->pad != 0) {
        blob_free(in);
    }

    //perform batchnorm if needed
    if (p->bn_mean != NULL) {

        //load batchnorm mean and variance
        float* mean = load_1d(p->bn_mean, out->d);
        float* var = load_1d(p->bn_var, out->d);

        //batchnorm
        for (int o = 0; o < out->d; o++) {
            for (int m = 0; m < out->h; m++) {
                for (int n = 0; n < out->w; n++) {
                    blob_data(out, o, m, n) = (blob_data(out, o, m, n) - mean[o])
                                              / sqrtf(var[o] + p->bn_eps);
                }
            }
        }

        //free mean and variance
        free(mean);
        free(var);
    }

    //perform scale if needed
    if (p->scale != NULL) {
        //load scale parameters
        float* scale = load_1d(p->scale, out->d);
        float* scale_bias = load_1d(p->scale_bias, out->d);

        //scale
        for (int o = 0; o < out->d; o++) {
            for (int m = 0; m < out->h; m++) {
                for (int n = 0; n < out->w; n++) {
                    blob_data(out, o, m, n) = blob_data(out, o, m, n) * scale[o] + scale_bias[o];
                }
            }
        }

        //free parameters
        free(scale);
        free(scale_bias);
    }

    //perform relu
    if (p->relu == true) {
        for (int i = 0; i < blob_size(out); i++) {
            out->data[i] = fmaxf(0.0f, out->data[i]);
        }
    }

    //return output
    return out;
}
