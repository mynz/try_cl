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

	scalers[15] = get_num_groups(0);
	scalers[16] = get_num_groups(1);


	for ( int i = 0; i < 32; ++i ) {
	  /* scalers[i] = get_global_id(0); */
	}

	int job_width = kWidth / get_global_size(0);

	/* scalers[30] = get_global_size(0); */
	/* scalers[31] = job_width; */


	scalers[0] = get_global_size(0);
	scalers[1] = get_local_size(0);

	int x_slide  = kWidth / get_global_size(0);
	int x_offset = x_slide * get_global_id(0);

	int y_slide  = kWidth / get_global_size(1);
	int y_offset = y_slide * get_global_id(1);

	for ( int y = y_offset; y < y_offset + y_slide; ++y ) {
		for ( int x = x_offset; x < x_offset + x_slide; ++x ) {
			int i = x + y * kWidth;
			pixels[i] = (uchar4)(get_global_id(0), get_global_id(1), 0, 255).zyxw;
		}
	}
}

/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
