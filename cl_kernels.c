//
//  cl_setup.c
//  
//
//  Created by Jorrit Olthuis on 27/12/2018.
//

#include "cl_kernels.h"

/** Sets up OpenCL stuff to be able to run kernel_path
    - Detects devices
    - Creates a context
    - Creates command queue
    - Loads (and builds) kernel source
    - Creates the kernel with name
 */
cl_struct* init_device(char* kernel_path, char* name)
{
    cl_struct output;
    
    cl_uint dev_cnt = 0;
    clGetPlatformIDs(0, 0, &dev_cnt);
    
    cl_platform_id platform_ids[dev_cnt];
    clGetPlatformIDs(dev_cnt, platform_ids, NULL);
    
    // Connect to a compute device
    int gpu = 1;
    int err = clGetDeviceIDs(platform_ids[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &output.device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
    
    // Create a compute context
    output.context = clCreateContext(0, 1, &output.device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    
    // Create a command commands
    output.commands = clCreateCommandQueue(context, &output.device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
    
    // Create the compute program from the source file
    char *KernelSource;
    long lFileSize;
    
    lFileSize = LoadOpenCLKernel(kernel_path, &KernelSource);
    if( lFileSize < 0L ) {
        perror("File read failed");
        return 1;
    }
    
    output.program = clCreateProgramWithSource(output.context, 1, (const char **) & KernelSource, NULL, &err);
    if (!output.program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }
    
    // Build the program executable
    err = clBuildProgram(output.program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(output.program, &output.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
    
    // Create the compute kernel in the program we wish to run
    //
    output.kernel = clCreateKernel(output.program, name, &err);
    if (!output.kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
}

/** Releases memory objects of kernel_objects */
void close_device(cl_struct* kernel_objects)
{
    clReleaseProgram(kernel_objects->program);
    clReleaseKernel(kernel_objects->kernel);
    clReleaseCommandQueue(kernel_objects->commands);
    clReleaseContext(kernel_objects->context);
}
