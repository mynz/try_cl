/* vim: set ft=c: */

/* __kernel void hello(void)  */
__kernel void hello(__global char *str) 
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
}
