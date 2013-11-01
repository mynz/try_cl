/* vim: set ft=c: */

const sampler_t s_nearest = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t s_linear  = CLK_FILTER_LINEAR  | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

#define kWidth 512
#define infinity (100000000.f)

float2 to_world(int x, int y)
{
	return (float2) ((float)x / kWidth * 2.f - 1.0f,
					((float)y / kWidth * 2.f - 1.0f));
}

typedef struct {
	float3 orig, dir;
} Ray;

typedef struct {
	float3 center;
	float radius;
} Sphere;

float ray_sphere(Sphere sphere, Ray ray)
{
	float3 v = sphere.center - ray.orig;
	float b = dot(v, ray.dir);
	float disc = b*b - dot(v, v) + sphere.radius * sphere.radius;

	if (disc < 0) return infinity;
	float d = sqrt(disc);
	float t2 = b + d;
	if (t2 < 0) return infinity;
	float t1 = b - d;
	return (t1 > 0 ? t1 : t2);
}

uchar4 to_color_from_normal(float3 norm)
{
	/* float3 f3 = norm * (float3)(127) + (float3)(127); */
	float3 f3 = (norm * (float3)(0.5) + (float3)(0.5)) * (float3)(255);
	return (uchar4)(convert_uchar3(f3), 255);
}

/**
 *
 */
__kernel void hello(
		  __global char *str
		, __global float *mat
		, __read_only image2d_t image
		, __global uchar4 *outImage
		, __constant Sphere *sphere_array
		, const int numSpheres
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

#if 0 // ok
	uchar4 col = (uchar4)(0, 10, 100, 0);
	for ( int i = 0; i < 3 * 256 * 256; ++i ) {
		outImage[i] = col;
	}
#endif


#if 0 // ray tracing.
	const uchar4 red  = (uchar4)(255, 0, 0, 0);
	const uchar4 blue = (uchar4)(0, 255, 0, 0);
	const float  kRad = 0.4f;

	for ( int sy = 0; sy < kWidth; ++sy ) {
		for ( int sx = 0; sx < kWidth; ++sx ) {

			int idx = sx * kWidth + sy;
			float2 w = to_world(sx, sy);
			float len2 = dot(w, w);

			uchar4 col = len2 < (kRad * kRad) ? red : blue;
			outImage[idx] = col;
		}
	}
#endif

#if 1 // ray tracing.
	const uchar4 red  = (uchar4)(0, 0, 255, 255);
	const uchar4 blue = (uchar4)(255, 0, 0, 255);
	const float  kRad = 0.4f;

	float3 camera_pos = (float3)(0, 0, 0);

	for ( int sy = 0; sy < kWidth; ++sy ) {
		for ( int sx = 0; sx < kWidth; ++sx ) {

			float2 w = to_world(sx, sy);

			float3 screen_center = (float3)(w, -1.f);
			float3 dir = normalize(screen_center - camera_pos);

			Ray ray;
			ray.orig = camera_pos;
			ray.dir  = dir;

			Sphere sp;
			uchar4 col = blue;
			for ( int s = 0; s < numSpheres; ++s ) {
				sp = sphere_array[s];
				float d = ray_sphere(sp, ray);
				if ( d != infinity ) {
					float3 nrm = normalize((camera_pos + d * dir) - sp.center);
					/* col = (uchar4)(convert_uchar3(nrm * (float3)(255, 255, 255)), 255); // XXX */
					col = to_color_from_normal(nrm);
				}
			}

			int idx = sy * kWidth + sx;
			outImage[idx] = col;
		}
	}
#endif

}
/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
