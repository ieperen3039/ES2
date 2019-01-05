#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#define CHECK_ERR(err) if((err) != CL_SUCCESS) {printf("Error line %d: Failed to generate buffer! %d\n", __LINE__, (err));exit(1);}

#ifndef bool
#define true (!0)
#define false 0
#define bool int
#endif

#include "blob.h"

typedef struct {

	//number of outputs
	int num_out;

	//kernels sizes
	int Ky;
	int Kx;

	//strides
	int Sy;
	int Sx;

	//padding
	int pad;

	//group
	int group;

	//weights
	const char* weights;

	//bias term
	const char* bias;

	//scaling terms
	const char* scale;
	const char* scale_bias;

	//batch norm
	const char* bn_mean;
	const char* bn_var;
	float bn_eps;

	//relu
	bool relu;

	//fully connected
	bool fc;

} conv_param_t;

void
cpu_kernel(const BLOB* in, const conv_param_t* p, int kernelYSize, int kernelXSize, const BLOB* out, const BLOB* w);

void
gpu_kernel(const BLOB* in, const conv_param_t* p, int kernelYSize, int kernelXSize, const BLOB* out, const BLOB* w);

//perform convolution
BLOB* convolution(BLOB* in, conv_param_t* p);

#endif
