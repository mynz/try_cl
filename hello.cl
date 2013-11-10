/* vim: set ft=c: */

#define kWidth 512
#define infinity (100000000.f)

#define red     (float3)(1    , 0    , 0)
#define blue    (float3)(0    , 0    , 1)
#define yellow  (float3)(1    , 1    , 0)
#define gray    (float3)(0.5f , 0.5f , 0.5f)

const sampler_t s_nearest = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;
const sampler_t s_linear  = CLK_FILTER_LINEAR  | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE;

typedef struct {
	float3 orig, dir;
} Ray;

typedef struct {
	float4 center; // w = radius
	float3 clolor;
} Sphere;

typedef struct {
	int num_spheres;
	int num_colors;
} Misc;

inline float3 lerp(float3 a, float3 b, float r)
{
	return a * (1.f - r) + b * r;
}

inline float2 to_world(int x, int y)
{
	return (float2) ((float)x / kWidth * 2.f - 1.0f,
					((float)y / kWidth * 2.f - 1.0f));
}

inline float ray_sphere(Sphere sphere, Ray ray)
{
	float3 v = sphere.center.xyz - ray.orig;
	float b = dot(v, ray.dir);
	float radius = sphere.center.w;
	float disc = b*b - dot(v, v) + radius * radius;

	if (disc < 0) return infinity;
	float d = sqrt(disc);
	float t2 = b + d;
	if (t2 < 0) return infinity;
	float t1 = b - d;
	return (t1 > 0 ? t1 : t2);
}

inline uchar4 from_normal_to_uchar4(float3 norm)
{
	float3 f3 = (norm * (float3)(0.5) + (float3)(0.5)) * (float3)(255);
	return (uchar4)(convert_uchar3(f3), 255);
}

inline float3 phong(float3 nrm, float3 eye_dir, float3 diffuse_color)
{
	const float3 light_dir = normalize((float3)(1, 0.8, 0.55));
	float3 hlf = normalize(light_dir - eye_dir);

	float HN = max(dot(hlf, nrm), 0.000000000f);
	float3 diff = diffuse_color * dot(nrm, light_dir);
	float3 spec = 1.f * pow(HN, 32);
	float3 out = diff + spec;
	return out;
}

/**
 *
 */
__kernel void hello(
		  __global char *str
		, __global float *mat
		/* , __read_only image2d_t image */
		/* , __global uchar4 *outImage */
		/* , __constant Sphere *sphere_array */
		/* , __constant float3* color_table */
		/* , const Misc misc */
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


	return;

}
/* #define CL_BUILD_PROGRAM_FAILURE                    -11 */
/* #define CL_INVALID_KERNEL_ARGS                      -52 */
