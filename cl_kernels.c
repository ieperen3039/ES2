
#include "cl_kernels.h"

/** Sets up OpenCL stuff to be able to run kernel_path
    - Detects devices
    - Creates a context
    - Creates command queue
    - Loads (and builds) kernel source
    - Creates the kernel with name
 */
cl_struct* init_device(char* kernel_path, char* name) {
    // Allocate memory on the heap
    cl_struct* p_output = (cl_struct*) malloc(sizeof(cl_struct));
    cl_struct output = *p_output;

    cl_uint dev_cnt = 0;
    clGetPlatformIDs(0, 0, &dev_cnt);

    cl_platform_id platform_ids[dev_cnt];
    clGetPlatformIDs(dev_cnt, platform_ids, NULL);

    // Connect to a compute device
    int gpu = 1;
    int err = clGetDeviceIDs(platform_ids[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &output.device_id,
                             NULL);
    if (err != CL_SUCCESS) {
        printf("Error: Failed to create a device group!\n");
        exit(1);
    }

    // Create a compute context
    output.context = clCreateContext(0, 1, &output.device_id, NULL, NULL, &err);
    if (!output.context) {
        printf("Error: Failed to create a compute context!\n");
        exit(1);
    }

    // Create a command commands
    output.commands = clCreateCommandQueue(output.context, output.device_id, 0, &err);
    if (!output.commands) {
        printf("Error: Failed to create a command commands!\n");
        exit(1);
    }

    // Create the compute program from the source file
    char* KernelSource;
    long lFileSize;

    lFileSize = loadOpenCLKernel(kernel_path, &KernelSource);
    if (lFileSize < 0L) {
        perror("File read failed");
        exit(1);
    }

    output.program = clCreateProgramWithSource(output.context, 1, (const char**) &KernelSource, NULL, &err);
    if (!output.program) {
        printf("Error: Failed to create compute program!\n");
        exit(1);
    }

    // Build the program executable
    err = clBuildProgram(output.program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[16384];
        printf("Error: Failed to build program executable! %d\n", err);
        // Determine which error is returned:
        //printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", CL_INVALID_PROGRAM, CL_INVALID_VALUE, CL_INVALID_DEVICE, CL_INVALID_BINARY, CL_INVALID_BUILD_OPTIONS, CL_INVALID_OPERATION, CL_COMPILER_NOT_AVAILABLE, CL_DEVICE_COMPILER_AVAILABLE, CL_BUILD_PROGRAM_FAILURE, CL_INVALID_OPERATION, CL_OUT_OF_HOST_MEMORY);
        clGetProgramBuildInfo(output.program, output.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    //
    output.kernel = clCreateKernel(output.program, name, &err);
    if (!output.kernel || err != CL_SUCCESS) {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    return p_output;
}

/** Releases memory objects of kernel_objects */
void close_device(cl_struct* kernel_objects) {
    clReleaseProgram(kernel_objects->program);
    clReleaseKernel(kernel_objects->kernel);
    clReleaseCommandQueue(kernel_objects->commands);
    clReleaseContext(kernel_objects->context);

    free(kernel_objects);
}

static long loadOpenCLKernel(char const* path, char** buf) {
    FILE* fp;
    size_t fsz;
    long off_end;
    int rc;

    /* Open the file */
    fp = fopen(path, "r");
    if (fp == NULL) {
        return NO_FILE;
    }

    /* Seek to the end of the file */
    rc = fseek(fp, 0L, SEEK_END);
    if (rc != 0) {
        return NO_FILE;
    }

    /* Byte offset to the end of the file (size) */
    if (0 > (off_end = ftell(fp))) {
        return NO_FILE;
    }
    fsz = (size_t) off_end;

    /* Allocate a buffer to hold the whole file */
    *buf = (char*) malloc(fsz + 1);
    if (*buf == NULL) {
        return NO_FILE;
    }
    /* Make sure the buffer is NUL-terminated, just in case */
    (*buf)[fsz] = '\0';

    /* Rewind file pointer to start of file */
    rewind(fp);

    /* read file into buffer */
    size_t fileSize = fread(*buf, 1, fsz, fp);
    if (fileSize != fsz) {
        free(*buf);
        return NO_FILE;
    }

    /* Close the file */
    if (fclose(fp) == EOF) {
        free(*buf);
        return NO_FILE;
    }

    /* Return the file size */
    return (long) fsz;
}
