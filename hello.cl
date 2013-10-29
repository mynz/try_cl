/* vim: set ft=c: */

/* __kernel void hello(void)  */
__kernel void hello(__global char *str, __global float *mat) 
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
		mat[i] *= 3.f;
		/* mat[i] = mat[i] * 2.f; */
	}

	mat[0] = get_global_id(0);
	/* mat[0] = (1.0f == 1.0f); */
	/* mat[15] = ((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x; */
	/* mat[15] = as_float(((float4)(1, 1, 1, 1) == (float4)(1, 1, 1, 1)).x); */
}
