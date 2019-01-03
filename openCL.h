//
// Created by s152717 on 31-12-2018.
//

#ifndef KITTEN_OPENCL_H
#define KITTEN_OPENCL_H

#ifdef WIN32
#include <CL/opencl.h>
#elif __linux__
#include <OpenCL/cl.h>
#endif

#endif //KITTEN_OPENCL_H
