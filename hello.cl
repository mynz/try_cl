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


	scalers[0] = get_global_size(0);
	scalers[1] = get_local_size(0);

	int slide  = kWidth / get_global_size(0);
	int offset = slide * get_global_id(0);

	for ( int y = offset; y < offset + slide; ++y ) {
		for ( int x = 0; x < kWidth; ++x ) {
			int i = x + y * kWidth;


			pixels[i] = (uchar4)(get_global_id(0), 0, 0, 255).zyxw;
			/* pixels[i] = (uchar4)(155, 0, 0, 255).zyxw; */
		}
	}

}

/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
