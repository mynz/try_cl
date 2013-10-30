/* vim: set ft=c: */

const sampler_t s_nearest = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t s_linear  = CLK_FILTER_LINEAR  | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

__kernel void hello(
		  __global char *str
		, __global float *mat
		, __read_only image2d_t image
		, __global uchar4 *outImage
		/* , __global float *outImage */
		) 
{
	str[0] = 'H';
	str[1] = 'e';
	str[2] = 'l';
	str[3] = 'l';
	str[4] = 'o';

	char w[] = "World";
	int n = sizeof(w);
	for ( int i = 0; i < n; ++i ) {
		str[i + 5] = w[i];
	}
	/* str[15] = ('a' == 'a'); */
	str[15] = ((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x;

	////

	for ( int i = 0; i < 16; ++i ) {
		/* mat[i] = i * 100.0f; */
		mat[i] += 3.f;
		/* mat[i] = mat[i] * 2.f; */
	}

	mat[15] = get_global_id(0);
	/* mat[0] = (1.0f == 1.0f); */
	/* mat[15] = ((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x; */
	/* mat[15] = as_float(((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x); */


#if 1
	int x, y;
	for ( y = 0; y < 4; ++y ) {
		for ( x = 0; x < 4; ++x ) {
			float2 pos = (float2)(x, y);
			float4 col = read_imagef(image, s_linear, pos);
			int i = 4 * y + x;
			mat[i] = col.x;
		}
	}
#endif

#if 1
	uchar4 col = (uchar4)(0, 10, 100, 0);
	for ( int i = 0; i < 3 * 256 * 256; ++i ) {
		outImage[i] = col;
	}
#endif

}
/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
