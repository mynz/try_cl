#define __CL_ENABLE_EXCEPTIONS

#include "cl.hpp"

#include <CL/OpenCL.h>
#include <iostream>

using namespace std;


const char * helloStr  = "__kernel void "
                         "hello(void) "
                         "{ "
                         "  "
                         "} ";

						
void printPlatformInfo(const cl::Platform &p)
{
	cl_platform_info infos[] = {
		CL_PLATFORM_PROFILE,
		CL_PLATFORM_VERSION,
		CL_PLATFORM_NAME,
		CL_PLATFORM_VENDOR,
		CL_PLATFORM_EXTENSIONS,
	};

	std::string str;
	size_t n = sizeof(infos) / sizeof(infos[0]);
	// printf("num platforms: %d\n", platforms.size());
	for ( size_t i = 0; i < n; ++i ) {
		p.getInfo(infos[i], &str);
		printf("info[%u]: %s\n", i, str.c_str());
	}
}

int main(void)
{
	cl_int err = CL_SUCCESS;
	try {

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		if (platforms.size() == 0) {
			std::cout << "Platform size 0\n";
			return -1;
		}

		cl_context_properties properties[] = 
		{ CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};

		cl::Context context(CL_DEVICE_TYPE_DEFAULT, properties); 

		std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

#if 1
		printf("num platforms: %d\n", platforms.size());
		printPlatformInfo(platforms[0]);
#endif

		cl::Program::Sources source(1,
				std::make_pair(helloStr, strlen(helloStr)));
		cl::Program program_ = cl::Program(context, source);
		program_.build(devices);

		cl::Kernel kernel(program_, "hello", &err);

		cl::Event event;
		cl::CommandQueue queue(context, devices[0], 0, &err);
		queue.enqueueNDRangeKernel(
				kernel, 
				cl::NullRange, 
				cl::NDRange(4,4),
				cl::NullRange,
				NULL,
				&event); 

		event.wait();
	}
	catch (cl::Error err) {
		std::cerr 
			<< "ERROR: "
			<< err.what()
			<< "("
			<< err.err()
			<< ")"
			<< std::endl;
	}

	cout << "[Done the function]" << endl;
	return EXIT_SUCCESS;
}
