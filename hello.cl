/* vim: set ft=c: */

const sampler_t s_nearest = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t s_linear  = CLK_FILTER_LINEAR  | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

/* __kernel void hello(void)  */
__kernel void hello(
		__global char *str,
		__global float *mat
		/* , __global image2d_t image */
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


#if 0

	float4 f4 = read_imagef(image, s_nearest, (float2)(0.0f, 0.5f));
	/* float4 f4 = (float4)(0, 0, 0, 0); */
	float f = f4.x;
	mat[0] = f;
	/* mat[0] = (float)(f4); */


	/* mat[0] = read_imagef(image, s_nearest, (float2)(0.0f, 0.5f)); */
	/* mat[1] = read_imagef(image, s_nearest, (float2)(0.5f, 0.5f)); */
	/* mat[2] = read_imagef(image, s_nearest, (float2)(0.25f, 0.5f)); */
#endif

}
