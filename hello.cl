/* vim: set ft=c: */


/**
 *
 */
__kernel void hello(
		  __global float  * scalers
		, __global float4 * vectors
		) 
{
	scalers[ get_global_id(0) ] = 777;
	/* scalers[ get_local_id(0) ] = 123; */


	   scalers[15] = get_num_groups(0);

	for ( int i = 0; i < 32; ++i ) {
	  /* scalers[i] = get_global_id(0); */
	}
}

/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
