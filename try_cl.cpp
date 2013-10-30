#define __CL_ENABLE_EXCEPTIONS

#if defined(WIN32)
#pragma warning(disable:4996)
#endif


#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include "cl.hpp"

#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

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
		printf("info[%lu]: %s\n", i, str.c_str());
	}
}


void printDeviceInfo(const cl::Device &dev)
{
	cl_platform_info infos[] = {
		CL_DEVICE_NAME,
		CL_DEVICE_VENDOR,
		CL_DRIVER_VERSION,
		CL_DEVICE_PROFILE,
		CL_DEVICE_VERSION,
		CL_DEVICE_EXTENSIONS,
	};

	std::string str;
	size_t n = sizeof(infos) / sizeof(infos[0]);
	// printf("num platforms: %d\n", platforms.size());
	for ( size_t i = 0; i < n; ++i ) {
		dev.getInfo(infos[i], &str);
		printf("dev info[%lu]: %s\n", i, str.c_str());
	}

	puts("--");

	printf("  CL_DEVICE_MAX_COMPUTE_UNITS: %u\n", dev.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());
	printf("  CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: %u\n", dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>());
	printf("  CL_DEVICE_MAX_WORK_GROUP_SIZE: %lu\n", dev.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>());

	std::vector<size_t> itemsSizes = dev.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
	printf("CL_DEVICE_MAX_WORK_ITEM_SIZES[%lu]: %lu, %lu, %lu\n",
		   	itemsSizes.size(),
		   	itemsSizes[0],
		   	itemsSizes[1],
		   	itemsSizes[2]);

	puts("--");
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

		cl_device_type dt = 1 ? CL_DEVICE_TYPE_DEFAULT : CL_DEVICE_TYPE_CPU;

		cl::Context context(dt, properties); 

		std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

#if 1
		printf("num platforms: %lu\n", platforms.size());
		for ( size_t i = 0; i < platforms.size(); ++i ) {
			printPlatformInfo(platforms[i]);
		}

		printf("num devices: %lu\n", devices.size());
		for ( size_t i = 0; i < devices.size(); ++i ) {
			printDeviceInfo(devices[i]);
		}
#endif

#if 0
		cl::Program::Sources source(1,
				std::make_pair(helloStr, strlen(helloStr)));
#else
		string str;
		{
			std::stringstream ss;
			ifstream fin("hello.cl");
			ss << fin.rdbuf();
			str = ss.str();
		}

		cout << "code byte: " <<  str.size() << endl << str << endl;

		cl::Program::Sources source(1,
				std::make_pair(str.c_str(), str.size()));
#endif

		cl::Program program_ = cl::Program(context, source);
		program_.build(devices);

		cl::Kernel kernel(program_, "hello", &err);

		cl::Event event;
		cl::CommandQueue queue(context, devices[0], 0, &err);

#if 1 // XXX: hello_message
		char message[32];

		cl_mem memObj = clCreateBuffer(context(), CL_MEM_READ_WRITE, 
				sizeof(message), NULL, &err);
		assert( err == CL_SUCCESS );

		float mat[16];
		for ( int i = 0; i < 16; ++i ) {
			mat[i] = float(i);
		}

		cl::Buffer matObj(context, CL_MEM_READ_WRITE , sizeof(mat), NULL, &err);
		assert( err == CL_SUCCESS );

		err = queue.enqueueWriteBuffer( matObj, CL_TRUE, 0, sizeof(mat), (void*)mat);
		assert( err == CL_SUCCESS );

		err = clSetKernelArg(kernel(), 0, sizeof(cl_mem), &memObj);
		assert( err == CL_SUCCESS );

		err = kernel.setArg(1, matObj);
		// err = kernel.setArg(1, sizeof(mat), mat);
		assert( err == CL_SUCCESS );
#endif


#if 1 // image.
		float imgSrc[3*3] = {
			0, 1, 0,
			1, 0, 0,
			0, 1, 0,
		};

		cl::Image2D imgObj(context, CL_MEM_READ_ONLY,
				cl::ImageFormat(CL_R, CL_FLOAT),
				3, 3, 0, NULL, &err);

		cl::size_t<3> origin; origin[0] = origin[1] = origin[2] = 0;
		cl::size_t<3> region; region[0] = 3; region[1] = 3; region[2] = 1;

		err = queue.enqueueWriteImage(imgObj, CL_TRUE, origin, region, 
				0, 0, (void*)imgSrc);
		assert( err == CL_SUCCESS );

		kernel.setArg(2, imgObj);
#endif


#if 0
		queue.enqueueTask(kernel, NULL, &event);
#else
		queue.enqueueNDRangeKernel(
				kernel, 
				cl::NullRange,  // must be null in current OpenCL verison.
				cl::NDRange(1),
				cl::NullRange,
				NULL,
				&event); 
#endif

		event.wait();


#if 1 // XXX: hello_message
		err = clEnqueueReadBuffer(queue(), memObj, CL_TRUE, 0,
				sizeof(message), message, 0, NULL, &event());

		event.wait();

		cout << "read err: " << err << endl;
		cout << "RESULT MESSAGE IS [" << message << "]" << endl;
		cout << "read: " << message << endl;

		printf("message[15]: '%d'\n", message[15]);


		for ( int i = 0; i < 16; ++i ) {
			cout << "mat[" << i << "]: " << mat[i] << endl;
		}

		err = clEnqueueReadBuffer(queue(), matObj(), CL_TRUE, 0,
				sizeof(mat), &mat[0], 0, NULL, NULL);
		// err = queue.enqueueReadBuffer(matObj, CL_TRUE, 0, sizeof(mat), mat, NULL, &event);
		assert( err == CL_SUCCESS );

		// Sleep(3000);

		for ( int i = 0; i < 16; ++i ) {
			cout << "mat[" << i << "]: " << mat[i] << endl;
		}
#endif



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
