/* vim: set ft=c: */

/* __kernel void hello(void)  */
__kernel void hello(__global char *str, __global float *mat) 
{ 
	/* printf("hello from kernel"); */
	/* int i = 7; */
	/* int j = 8; */
	/* int d = i + j; */

	str[0] = 'H';
	str[1] = 'e';
	str[2] = 'l';
	str[3] = 'l';
	str[4] = 'o';

	/* str[15] = ('a' == 'a'); */
	str[15] = ((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x;

	for ( int i = 0; i < 16; ++i ) {
		mat[i] = i * 100.0f;
		/* mat[i] *= 2.f; */
		/* mat[i] = mat[i] * 2.f; */
	}

	/* mat[0] = get_global_id(0); */
	mat[0] = (1.0f == 1.0f);
	mat[15] = ((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x;
	/* mat[15] = as_float(((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x); */
}
