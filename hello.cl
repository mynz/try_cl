/* vim: set ft=c: */


/**
 *
 */
__kernel void hello(
		  __global float  * scalers
		, __global float4 * vectors
		) 
{
	for ( int i = 0; i < 32; ++i ) {
	  scalers[i] = i;
	}
}

/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
