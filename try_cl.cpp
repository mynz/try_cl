#include "cl.hpp"

#include <CL/OpenCL.h>
#include <iostream>

using namespace std;

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
