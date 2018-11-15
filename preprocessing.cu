#include "logging.h"
#include "image_util.h"

//CPU version
void cpu_preprocess(IMG* img){
    //Subtract mean RGB values, scale with 0.017, and swap RGB->BGR
    for(int y=0;y<img->height;y++)
        for(int x=0;x<img->width;x++){
            float R            = (img->data[0][y][x]-123.680f)*0.017f; //R
            img->data[1][y][x] = (img->data[1][y][x]-116.779f)*0.017f; //G
            img->data[0][y][x] = (img->data[2][y][x]-103.939f)*0.017f; //B
            img->data[2][y][x] = R;
         }
}


//GPU device code (what the threads execute)
__global__ void gpu_device_preprocess(float** channel_data, int img_width){
    //This code gets executed by each thread in the GPU
    //First step is identifying which thread we are

    // The ids of this thread within our block
    //unsigned int local_x = threadIdx.x; //x coordinate *within* the block this thread maps to
    //unsigned int local_y = threadIdx.y; //y coordinate *within* the block this thread maps to
    //unsigned int local_z = threadIdx.z; //z coordinate *within* the block this thread maps to

    // The global ids
    unsigned int global_x = blockIdx.x*blockDim.x + threadIdx.x;  //blockid*width_of_a_block + local_x
    unsigned int global_y = blockIdx.y*blockDim.y + threadIdx.y;
    //note that global_z==local_z! The grid of blocks is only 2D (x and y), so no blocks in the channel dimension!
    unsigned int global_z = threadIdx.z;

    //load single pixel from global memory into register
    //HINT: the global memory is very slow, so if you have multiple uses of the same pixel, it might be smart to look into the "shared memory".
    //Here however there is only one use of each pixel, so nothing to be gained from using shared memory
    //recall that the XxY arrays were flattend to a 1D array, so we have to do our own address calculations to get to the right pixel;
    float value = channel_data[global_z][global_y*img_width+global_x];

    //each channel (Z) needs to correct with a different value
    float mean[3]={
        123.680f,
        116.779f,
        103.939f
    };

    //correct by subtracting the correct mean for this channel and scaling by the factor 0.017
    value= (value-mean[global_z]) * 0.017f;

    //time to commit the value to the global memory
    //note that we swap RGB to BGR, as required by the preprocessing, by inverting bit 1
    unsigned int z = (global_z&(~0x2))|(~(global_z&0x2));
    channel_data[z][global_y*img_width+global_z] = value;
}

//GPU host code (called from the CPU, copies data back and forth and launched the GPU thread)
void gpu_preprocess(IMG* img){

    //GPU code

    /*
     *  Our high level strategy to perform the preprocessing in parallel in this example is to split each of the 3 channels into a number of blocks (numBlocks).
     *  Each block contains thus channel_length/numBlocks elements
     *  These blocks will be mapped to the Streaming Multiprocessors of the GPU.
     *  For each block the preprocessing is performed
    */

    //let's decide on a number of blocks per channel
    int numBlocksX=4;
    int numBlocksY=4;
    int threadsPerBlockX=img->width/numBlocksX; //NOTE: this should have remainder==0 !!
    int threadsPerBlockY=img->width/numBlocksY; //NOTE: this should have remainder==0 !!

    dim3 grid( numBlocksX, numBlocksY ); // numBlocksX x numBlocksY ( x 1)
    dim3 block(threadsPerBlockX, threadsPerBlockY, 3  ); // threadsPerBlockX x threadsPerBlockY x 3

    //Allocate 3 channels on the global memory of the GPU, and transfer data from CPU to GPU
    float* device_channels[3];
    for(int c=0;c<3;c++){

        //variable for holding return values of cuda functions
        cudaError_t err;

        //allocate the space of 1 complete channel (Height x Width)
        err=cudaMalloc(&device_channels[c], img->width*img->height*sizeof(float));

        //Here we check for errors of this cuda call
        //See logging.h for the implementation of this error check (it's not a default cuda function)
        cudaCheckError(err);

        //Now copy the channel from the CPU to the GPU (note: now directly wrapped in the error checking function)
        //also, the XY grid is flattend into a single 1D grid so only one cudaMalloc is needed per channel.
        //channels could of course also be flattened, but for the example we won't do that
        for(int y=0;y<img->height;y++)
            cudaCheckError(cudaMemcpy(&(device_channels[c][y*img->width]), img->data[c][y], img->width*sizeof(float), cudaMemcpyHostToDevice));
    }

    //Perform the preprocessing on the GPU
    gpu_device_preprocess<<< grid, block >>>(device_channels, img->width);

    //We use "peekatlasterror" since a kernel launch does not return a cudaError_t to check for errors
    cudaCheckError(cudaPeekAtLastError());

    //copy the processed image data back from GPU global memory to CPU memory
    for(int c=0;c<3;c++){
        for(int y=0;y<img->height;y++)
            cudaCheckError(cudaMemcpy(img->data[c][y], &(device_channels[c][y*img->width]), img->width*sizeof(float), cudaMemcpyDeviceToHost));

        //free the allocate GPU memory
        cudaCheckError(cudaFree(device_channels[c]));
    }
}
