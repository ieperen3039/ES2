//
//  preprocessing_kernel.c
//  
//
//  Created by Jorrit Olthuis on 27/12/2018.
//

//GPU kernel code
__kernel void gpu_device_preprocess(__local BLOB* output_blob, __local BLOB* input_blob){
    //This code gets executed by each thread in the GPU
    //First step is identifying which thread we are
    
    // The ids of this thread within our block (commented here since this particular kernel doesn't need them)
    //    unsigned int local_x = threadIdx.x; //x coordinate *within* the block this thread maps to
    //    unsigned int local_y = threadIdx.y; //y coordinate *within* the block this thread maps to
    //    unsigned int local_z = threadIdx.z; //z coordinate *within* the block this thread maps to
    
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
    index = global_z*img_height*img_width + global_y*img_width + global_x;
    float value = data_in[indexs];
    
    //each channel (Z) needs to correct with a different mean value
    float mean[3] = {123.680f, 116.779f, 103.939f};
    
    /* correct by subtracting the correct mean for this channel and scaling by a factor 0.017 */
    value = (value-mean[global_z]) * 0.017f;
    
    /*
     Time to commit the value to the global memory.
     Note that we swap RGB to BGR (2-z), as required by the preprocessing.
     If we did not need to swap, it would have been possible to just overwrite the input (compute in-place). However, now
     that we swap, we might destroy the input data of another thread, hence synchronisation or an extra output buffer is
     required. Since synchronisation between threadblocks is costly, and the global memory is large hence we just
     allocated an extra output buffer for this example
     */
    index = (2-global_z)*img_width*img_height + global_y*img_width + global_x;
    data_out[index] = value;
}
