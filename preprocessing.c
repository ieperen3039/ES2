#include "openCL.h"

#include "preprocessing.h"
#include "cl_kernels.h"

//CPU version
void cpu_preprocess(BLOB* img) {
	//Subtract mean RGB values, scale with 0.017, and swap RGB->BGR
    for (int y = 0; y < img->h; y++) {
		for (int x = 0; x < img->w; x++) {
			float R = (blob_data(img,0,y,x)-123.680f)*0.017f; //R
			blob_data(img, 1,y,x) = (blob_data(img,1,y,x)-116.779f)*0.017f;//G
			blob_data(img, 0,y,x) = (blob_data(img,2,y,x)-103.939f)*0.017f;//B
			blob_data(img, 2,y,x) = R;
		}
	}
}

//GPU host code (called from the CPU, copies data back and forth and launches the GPU thread)
void gpu_preprocess(BLOB* img){
    /*
     The high level strategy is to map the pixels of the image to threads in the GPU, and have each thread preprocess a single pixel.
     The GPU supports a 3-dimenional grid of 3-dimensional blocks, which can be a good fit for our channels x height x width image.
     In fact, we will make 3 dimensional thread blocks, which are 3 deep in the z dimension to map to the RGB channels.
     The x and y dimensions will be mapped to the width and height of the image.
     
     Each threadblock can only support a limited number of threads (less than we have pixels). Therefore, we need to
     divide the image in multiple blocks using the grid. The grid will be 2 dimensional (only x and y), since the Z
     dimension is already captured inside the threadblock
     */
    
    int err = CL_SUCCESS;
    
/***********
    // CUDA code, has not directly been converted to something in OpenCL
    
    // Divide the X and Y dimensions of the image into a number of blocks here
    int numBlocksX=16;
    int numBlocksY=16;
    
    //The number of blocks determines the number of threads our blocks need to have in both X and Y to cover the complete image
    int threadsPerBlockX=img->w/numBlocksX;  //NOTE: this should have remainder==0 for this code!!
    int threadsPerBlockY=img->h/numBlocksY;  //NOTE: this should have remainder==0 for this code!!
    //int threadsPerBlockZ=3;                //not required, but correct ;)
    
    printf("Grid dimensions %d x %d (x 1)\n", numBlocksX,numBlocksY);
    printf("Block dimensions %d x %d x 3\n", threadsPerBlockX,threadsPerBlockY);
    
    //To specify the grid and block dimensions, cuda uses this special "dim3" datatype.
    //Note that our grid is actually only 2D (as far as we are concerned), so we set the z-dimension to be 1
    dim3 grid( numBlocksX, numBlocksY, 1 );             // numBlocksX x numBlocksY ( x 1)
    dim3 block(threadsPerBlockX, threadsPerBlockY, 3);  // threadsPerBlockX x threadsPerBlockY x 3
 
 ***********/
    
    /* Initialise OpenCL kernel */
    // Note: this is only possible because the kernel environment is on the heap
    cl_struct* p_kernel_env = init_device((char *) "preprocessing_kernel.cl", (char *) "gpu_device_preprocess");
    
    /* Copy image data from the CPU to the GPUs global memory */
    
    // Prepare memory
    size_t blob_size = blob_bytes(img);
    
    /* Set arguments */
    // Input
    printf("READY\n");
    cl_mem input_blob = clCreateBuffer(p_kernel_env->context,
                                       CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                       blob_size,
                                       img->data,
                                       &err);
    printf("...\n");
    if(err != CL_SUCCESS) {
        printf("Error line %d: Failed to generate buffer! %d\n", __LINE__, err);
        exit(1);
    }
    printf("SUCCESS ONCE!\n");
    // Output
    cl_mem output_blob = clCreateBuffer(p_kernel_env->context,
                                        CL_MEM_WRITE_ONLY,
                                        blob_size,
                                        NULL,
                                        &err);
    if(err != CL_SUCCESS) {
        printf("Error line %d: Failed to generate buffer! %d\n", __LINE__, err);
        exit(1);
    }
    
    // Set buffers as kernel arguments
    err |= clSetKernelArg(p_kernel_env->kernel, 0, sizeof(cl_mem), (void *)&output_blob); // Output pointer
    err |= clSetKernelArg(p_kernel_env->kernel, 1, sizeof(cl_mem), (void *)&input_blob); // Blob input
    
    if(err != CL_SUCCESS) {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }
    
    /* Perform the preprocessing on the GPU */
    
    size_t work_dim = 2;
    size_t global_work_size[work_dim]; // The total number of global work items is the product of all elements
    global_work_size[0] = 1;
    global_work_size[1] = 1;
    size_t local_work_size[work_dim]; // The number of work items in one work group
    local_work_size[0] = 1;
    local_work_size[1] = 1;
    
    printf("Preprocessing on GPU...\n");
    
    err = clEnqueueNDRangeKernel(p_kernel_env->commands,    // Command queue
                                 p_kernel_env->kernel,      // Kernel code
                                 work_dim,
                                 NULL,                      // global_work_offset, must be NULL according to documentation
                                 global_work_size,
                                 local_work_size,
                                 0, NULL, NULL);            // There are no events we need to wait for, before we can start
    
    if (err != CL_SUCCESS) {
        printf("Error: Failed to execute kernel! %d\n", err);
        exit(1);
    }
    
    /* Read output */
    
    // Load result from __local memory
    BLOB result_blob;
    err = clEnqueueReadBuffer(p_kernel_env->commands,   // Command queue
                              output_blob,             // Pointer to output arg from clCreateBuffer(), TODO
                              CL_TRUE,                  // Blocking read
                              0,                        // Read offset
                              blob_size,                // Number of bytes to read
                              &result_blob,             // Buffer to read data into
                              0, NULL, NULL);           // No events
    
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    /*
     TODO:
     Do something with result_blob
     */
    
    /* Release memory */
    close_device(p_kernel_env);
}
