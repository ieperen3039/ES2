#include "preprocessing.h"
#include "logging.h"

//CPU version
void cpu_preprocess(BLOB* img){
    //Subtract mean RGB values, scale with 0.017, and swap RGB->BGR
    for(int y=0;y<img->h;y++)
        for(int x=0;x<img->w;x++){
            float R              =  (blob_data(img,0,y,x)-123.680f)*0.017f; //R
            blob_data(img, 1,y,x) = (blob_data(img,1,y,x)-116.779f)*0.017f; //G
            blob_data(img, 2,y,x) = (blob_data(img,2,y,x)-103.939f)*0.017f; //B
            blob_data(img, 0,y,x) = R;
         }
}


//GPU device code (what the threads execute)
__global__ void gpu_device_preprocess(float* data){
    //This code gets executed by each thread in the GPU
    //First step is identifying which thread we are

    // The ids of this thread within our block (commented here since this particular kernel doesn't need them)
    //unsigned int local_x = threadIdx.x; //x coordinate *within* the block this thread maps to
    //unsigned int local_y = threadIdx.y; //y coordinate *within* the block this thread maps to
    //unsigned int local_z = threadIdx.z; //z coordinate *within* the block this thread maps to

    // The global ids (where is this thread in the total grid)
    unsigned int global_x = blockIdx.x*blockDim.x + threadIdx.x;  //blockid*width_of_a_block + local_x
    unsigned int global_y = blockIdx.y*blockDim.y + threadIdx.y;
    unsigned int global_z = blockIdx.z*blockDim.z + threadIdx.z; //NOTE: gridDim.z==1 and thus blockIdx.z==0 in this example!

    //The image height and width can be passed as an argument to this kernel, but they
    //can also be derived by using the grid and block dimensions in this case
    unsigned int img_width  = gridDim.x*blockDim.x;
    unsigned int img_height = gridDim.y*blockDim.y;

    //load single pixel from global memory into register
    //HINT: the global memory is very slow, so if you have multiple uses of the same pixel, it might be smart to look into the "shared memory".
    //Here however there is only one use of each pixel, so nothing to be gained from using shared memory
    float value = data[ global_z*img_height*img_width + global_y*img_width + global_x];

    //each channel (Z) needs to correct with a different mean value
    float mean[3]={
        123.680f,
        116.779f,
        103.939f
    };

    //correct by subtracting the correct mean for this channel and scaling by a factor 0.017
    value= (value-mean[global_z]) * 0.017f;

    //time to commit the value to the global memory
    //note that we swap RGB to BGR (2-z), as required by the preprocessing
    data[(2-global_z)*img_width*img_height + global_y*img_width + global_x]=value;
}

//GPU host code (called from the CPU, copies data back and forth and launched the GPU thread)
void gpu_preprocess(BLOB* img){

    //GPU code

    /*
     *  Our high level strategy to perform the preprocessing in parallel in this example is to split each of the 3 channels into a number of blocks (numBlocks).
     *  Each block contains thus channel_length/numBlocks elements
     *  These blocks will be mapped to the Streaming Multiprocessors of the GPU.
     *  For each block the preprocessing is performed
    */

    //let's decide on a number of blocks per channel
    int numBlocksX=16;
    int numBlocksY=16;
    int threadsPerBlockX=img->w/numBlocksX;  //NOTE: this should have remainder==0 !!
    int threadsPerBlockY=img->h/numBlocksY; //NOTE: this should have remainder==0 !!


    info("Grid dimensions %d x %d (x 1)\n", numBlocksX,numBlocksY);
    info("Block dimensions %d x %d x 3\n", threadsPerBlockX,threadsPerBlockY);

    dim3 grid( numBlocksX, numBlocksY, 1 ); // numBlocksX x numBlocksY ( x 1)
    dim3 block(threadsPerBlockX, threadsPerBlockY, 3); // threadsPerBlockX x threadsPerBlockY x 3

    //to save on some transfer overhead, the image data is flattened into a 1D array on the GPU
    //first allocate the space of 3 complete channels (Height x Width x Depth)

    //pointer to data on GPU
    float* device_data;

    //variable for holding return values of cuda functions
    cudaError_t err;

    //malloc on the GPU
    err=cudaMalloc(&device_data, blob_bytes(img));

    //check for errors (NOTE: this is not a standard cuda function. Check logging.h)
    cudaCheckError(err)

    //copy the data over to the GPU
    cudaCheckError(cudaMemcpy(device_data, img->data, blob_bytes(img), cudaMemcpyHostToDevice));

    //Perform the preprocessing on the GPU
    info("Preprocessing on GPU...\n");
    gpu_device_preprocess<<< grid, block >>>(device_data);

    //We use "peekatlasterror" since a kernel launch does not return a cudaError_t to check for errors
    cudaCheckError(cudaPeekAtLastError());

    //copy the processed image data back from GPU global memory to CPU memory
    cudaCheckError(cudaMemcpy(img->data, device_data, blob_bytes(img), cudaMemcpyDeviceToHost));

    //free the allocated GPU memory for this channel
    cudaCheckError(cudaFree(device_data));
}
