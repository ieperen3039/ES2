
#include "cl_kernels.h"
#include <unistd.h>

/** Sets up OpenCL environment to allow running the program defined by kernel_path
    - Detects devices
    - Creates a context
    - Creates command queue
    - Loads (and builds) kernel source
    - Creates the kernel with name
 */
cl_struct* init_device(char* kernel_path, char* name) {
    printf("Initializing device %s...\n", name);
    // Allocate memory on the heap
    cl_struct* p_output = (cl_struct*) malloc(sizeof(cl_struct));
    cl_struct output = *p_output;
    int error_code;

    cl_uint plaf_cnt = 0;
    clGetPlatformIDs(0, NULL, &plaf_cnt);
    printf("Found %d available platforms\n", plaf_cnt);

    cl_uint platform_list_length = plaf_cnt + 5;
    cl_platform_id platform_ids[platform_list_length];
    error_code = clGetPlatformIDs(platform_list_length, platform_ids, NULL);
    if (error_code != CL_SUCCESS) {
        printf("Error: Failed to get platform id. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    // Connect to a compute device
    cl_uint tgt_dev_cnt = 1;
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    
    printf("Connecting to %d devices...\n", tgt_dev_cnt);
    error_code = clGetDeviceIDs(platform_ids[0], deviceType, tgt_dev_cnt, &output.device_id, NULL);
    if (error_code != CL_SUCCESS) {
        printf("Error: Failed to create a device group. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    // Create a compute context
    printf("Creating context...\n");
    sleep(1);
    output.context = clCreateContext(NULL, tgt_dev_cnt, &output.device_id, NULL, NULL, &error_code);
    if (error_code != CL_SUCCESS || output.context == NULL) {
        printf("Error: Failed to create a compute context. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    // Create a command commands
    printf("Creating command queue...\n");
    sleep(1);
    output.commands = clCreateCommandQueue(output.context, output.device_id, 0, &error_code);
    if (error_code != CL_SUCCESS || output.commands == NULL) {
        printf("Error: Failed to create a command commands. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    // Create the compute program from the source file
    char* KernelSource;
    long lFileSize;

    printf("Writing program...\n");
    lFileSize = loadOpenCLKernel(kernel_path, &KernelSource);
    if (lFileSize < 0L) {
        perror("File read failed");
        exit(EXIT_FAILURE);
    }

    output.program = clCreateProgramWithSource(output.context, 1, (const char**) &KernelSource, NULL, &error_code);
    if (error_code != CL_SUCCESS || output.program == NULL) {
        printf("Error: Failed to create compute program. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    printf("Building program...\n");
    // Build the program executable
    error_code = clBuildProgram(output.program, 0, NULL, NULL, NULL, NULL);
    if (error_code != CL_SUCCESS) {
        size_t len;
        char buffer[16384];
        printf("Error: Failed to build program executable. (%s)\n", error_to_string(error_code));
        // Determine which error is returned:
        //printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", CL_INVALID_PROGRAM, CL_INVALID_VALUE, CL_INVALID_DEVICE, CL_INVALID_BINARY, CL_INVALID_BUILD_OPTIONS, CL_INVALID_OPERATION, CL_COMPILER_NOT_AVAILABLE, CL_DEVICE_COMPILER_AVAILABLE, CL_BUILD_PROGRAM_FAILURE, CL_INVALID_OPERATION, CL_OUT_OF_HOST_MEMORY);
        clGetProgramBuildInfo(output.program, output.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(EXIT_FAILURE);
    }

    // Create the compute kernel in the program we wish to run
    printf("Creating kernel...\n");
    output.kernel = clCreateKernel(output.program, name, &error_code);
    if (error_code != CL_SUCCESS || output.kernel == NULL) {
        printf("Error: Failed to create compute kernel. (%s)\n", error_to_string(error_code));
        exit(EXIT_FAILURE);
    }

    printf("Device %s initialized\n", name);
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

