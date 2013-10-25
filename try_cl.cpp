#define __CL_ENABLE_EXCEPTIONS

#include "cl.hpp"

#include <CL/OpenCL.h>
#include <iostream>


using namespace std;


#if 0

int main()
{
	cout << "[hello try_cl]" << endl;

#if 0
	cl_uint numPlatforms;
	cl_int result = clGetPlatformIDs(0, NULL, &numPlatforms);
	cout << "result: " << (result == CL_SUCCESS) << endl;
	cout << "numPlatforms: " << numPlatforms << endl;
#endif


#if 1
	// get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";
#endif


	return 0;
}
#endif

const char * helloStr  = "__kernel void "
                         "hello(void) "
                         "{ "
                         "  "
                         "} ";

int
main(void)
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

		// cl::Context context(CL_DEVICE_TYPE_CPU, properties); 
		cl::Context context(CL_DEVICE_TYPE_DEFAULT, properties); 

		std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

		cl::Program::Sources source(1,
				std::make_pair(helloStr,strlen(helloStr)));
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
