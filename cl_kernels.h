//
//  cl_setup.h
//  
//
//  Created by Jorrit Olthuis on 27/12/2018.
//

#ifndef cl_setup_h
#define cl_setup_h

#include <stdio.h>
#include <stdlib.h>
#include "openCL.h"

typedef struct {
    cl_device_id        device_id; // Compute device ID
    cl_context          context; // Compute context
    cl_command_queue    commands; // Compute command queue
    cl_program          program; // Compute program
    cl_kernel           kernel; // Compute kernel
} cl_struct;

cl_struct* init_device(char* kernel_path, char* name);
void close_device(cl_struct* kernel_objects);

#endif /* cl_setup_h */
