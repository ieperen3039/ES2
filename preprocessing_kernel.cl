//
//  preprocessing_kernel.cl
//  
//
//  Created by Jorrit Olthuis on 27/12/2018.
//

#include "blob.h"

//GPU kernel code
__kernel void gpu_device_preprocess(__global float* output_blob, __global float* input_blob){
    /*
     This code gets executed by each thread in the GPU.
     First step is identifying which thread we are.
     */
    
    /* The ids of this thread within our block (commented here since this particular kernel doesn't need them) */
//    unsigned int local_x = threadIdx.x; //x coordinate *within* the block this thread maps to
//    unsigned int local_y = threadIdx.y; //y coordinate *within* the block this thread maps to
//    unsigned int local_z = threadIdx.z; //z coordinate *within* the block this thread maps to
    
    // The global ids (where is this thread in the total grid)
    unsigned int global_x = get_global_id(0);
    unsigned int global_y = get_global_id(1);
    unsigned int global_z = get_global_id(2); // There is only one block in this direction
    
    printf("global location: (%d, %d, %d)\n", global_x, global_y, global_z);
    
    /*
     The image height and width can be passed as an argument to this kernel, but they can also be derived by using the
     grid and block dimensions in this case.
     */
    unsigned int blob_width  = get_global_size(0); //gridDim.x*blockDim.x;
    unsigned int blob_height = get_global_size(1); //gridDim.y*blockDim.y;
    
    /*
     load single pixel from global memory into register
     HINT: the global memory is very slow, so if you have multiple uses of the same pixel, it might be smart to look into the "shared memory".
     Here however there is only one use of each pixel, so nothing to be gained from using shared memory
     */
    int index = global_z*blob_height*blob_width + global_y*blob_width + global_x;
    float value = input_blob[index];
    
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
    index = (2-global_z)*blob_width*blob_height + global_y*blob_width + global_x;
    output_blob[index] = value;
}
