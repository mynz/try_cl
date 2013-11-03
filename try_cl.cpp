// #define __CL_ENABLE_EXCEPTIONS

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

#include <stdint.h>
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

bool saveImage(const char *filename, int w, int h, uint8_t *image)
{
	if ( (w % 8) || (h % 8) ) {
		printf("Error[saveImage]: width and height must be number of divided by 8");
		return false;
	}

#pragma pack(push, 1)
	struct /* __attribute__((packed)) */ BITMAPFILEHEADER {
	  uint16_t	bfType;
	  uint32_t	bfSize;
	  uint16_t	bfReserved1;
	  uint16_t	bfReserved2;
	  uint32_t	bfOffBits;
	} ;

	struct /* __attribute__((packed)) */ BITMAPINFOHEADER {
		uint32_t	biSize;
		int32_t		biWidth;
		int32_t		biHeight;
		uint16_t	iPlanes;
		uint16_t	iBitCount;
		uint32_t	biCompression;
		uint32_t	biSizeImage;
		int32_t		biXPixPerMeter;
		int32_t		biYPixPerMeter;
		uint32_t	biClrUsed;
		uint32_t	biClrImporant;
	};
#pragma pack(pop)

	const int kChannels = 4;

	size_t imageSize = w * h * kChannels;
	BITMAPFILEHEADER bheader = {
		0x4d42, // 'MB',
		imageSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
		0, 0, // reserved
		sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) // offset
	};

	BITMAPINFOHEADER binfo = {
		40,
		w, h,
		1,			// biPlanes
		kChannels == 4 ? 32 : 24,
		0,			// biCompression
		imageSize,	// biSizeImage
		0, 0,
		0, 0
	};

	bheader.bfSize = imageSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	FILE *fp = fopen(filename, "wb");
	if ( fp ) {
		fwrite(&bheader, 1, sizeof(bheader), fp);
		fwrite(&binfo, 1, sizeof(binfo), fp);
		fwrite(image, 1, imageSize, fp);
		fclose(fp);

		printf("saveImage has done successfully.\n");
		return true;
	}
	return false;
}

inline float RandF()
{
	return (float)rand() / RAND_MAX;
}

int main(void)
{
	cl_int err = CL_SUCCESS;

#if defined(__CL_ENABLE_EXCEPTIONS)
	try
#endif
	{

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

		string str;
		{
			std::stringstream ss;
			ifstream fin("hello.cl");
			ss << fin.rdbuf();
			str = ss.str();
		}

		cout << "code byte: " <<  str.size() << endl;
		// cout << "code byte: " <<  str.size() << endl << str << endl;

		cl::Program::Sources source(1,
				std::make_pair(str.c_str(), str.size()));

		cl::Program program_ = cl::Program(context, source);
		err = program_.build(devices);

		if ( err != CL_SUCCESS ) {
			std::string str;
			program_.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &str);
			cout << "buid error ocurred[" << err << "]: " << endl << str << endl;
			exit(-1);
		}

		assert( err == CL_SUCCESS );

		cl::Kernel kernel(program_, "hello", &err);

		cl::Event event;

		cl_command_queue_properties prop = 0 | CL_QUEUE_PROFILING_ENABLE ;
		cl::CommandQueue queue(context, devices[0], prop, &err);

#if 1 // XXX: hello_message

		cl::Buffer scalersBuf(context, CL_MEM_READ_WRITE , 32 * sizeof(float), NULL, &err);
		assert( err == CL_SUCCESS );
		err = kernel.setArg(0, scalersBuf);
		assert( err == CL_SUCCESS );

		// 
		cl::Buffer vectorsBuf(context, CL_MEM_READ_WRITE , 32 * 4 * sizeof(float), NULL, &err);
		assert( err == CL_SUCCESS );
		err = kernel.setArg(1, vectorsBuf);
		assert( err == CL_SUCCESS );

#endif

		cl::Event kernelEvent;
		err = queue.enqueueNDRangeKernel(
				kernel, 
				cl::NullRange,  // must be null in current OpenCL verison.
				cl::NDRange(12),
				cl::NDRange(3, 0, 0),
				NULL,
				&kernelEvent); 
		// cout << "err: " << err << endl;
		assert( err == CL_SUCCESS );

		err = kernelEvent.wait();

		if ( err != CL_SUCCESS ) {
			// it causes crash.
			cout << "!! Error: kernelEvent.wait = " << err << endl;
			exit(1);
		}

		assert( err == CL_SUCCESS );

		cl_ulong qcnt, scnt, ecnt;
		err = kernelEvent.getProfilingInfo(CL_PROFILING_COMMAND_QUEUED, &qcnt);
		assert( err == CL_SUCCESS );
		err = kernelEvent.getProfilingInfo(CL_PROFILING_COMMAND_START, &scnt);
		assert( err == CL_SUCCESS );
		err = kernelEvent.getProfilingInfo(CL_PROFILING_COMMAND_END, &ecnt);
		assert( err == CL_SUCCESS );
		const cl_ulong NSEC = 1000000000;
		double elpSec = (double(ecnt - qcnt) / NSEC);
		cout << "Done: enqueueNDRangeKernel(): " << elpSec << " Sec" << endl;


#if 1 // XXX: hello_message
		float scalers[32];
		err = clEnqueueReadBuffer(queue(), scalersBuf(), CL_TRUE, 0,
				32 * sizeof(float), &scalers[0], 0, NULL, NULL);
		assert( err == CL_SUCCESS );

		for ( int i = 0; i < 32; ++i ) {
			cout << "dst scalers[" << i << "]: " << scalers[i] << endl;
		}
#endif


	}
#if defined(__CL_ENABLE_EXCEPTIONS)
	catch (cl::Error err) {
		if ( err.err() == CL_BUILD_PROGRAM_FAILURE ) { /* do something */ }
		std::cerr 
			<< "ERROR: " << err.what()
			<< "(" << err.err() << ")"
			<< std::endl;
	}
#endif

	cout << "[Done the function]" << endl;
	return EXIT_SUCCESS;
}
