/* vim: set ft=c: */


#define kWidth 512

/**
 *
 */
__kernel void hello(
		  __global float  * scalers
		, __global float4 * vectors
		, __global uchar4 * pixels
		) 
{
	/* scalers[ get_global_id(0) ] = get_local_id(0) + 100 * get_global_id(0); */
	/* scalers[ get_local_id(0) ] = 123; */

	/* scalers[15] = get_num_groups(0); */
	/* scalers[31] = get_num_groups(0); // 6 */

	for ( int i = 0; i < 32; ++i ) {
	  /* scalers[i] = get_global_id(0); */
	}

	int job_width = kWidth / get_global_size(0);

	/* scalers[30] = get_global_size(0); */
	/* scalers[31] = job_width; */

	for ( int y = 0; y < kWidth; ++y ) {
		for ( int x = 0; x < kWidth; ++x ) {

			int i = x + y * kWidth;
			pixels[i] = (uchar4)(155, 0, 0, 255).zyxw;
		}
	}

}

/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
