//////////////////////////////////////////////////////////////
//
//	WHC template library. July 2011
//	Version 0.1
//
//////////////////////////////////////////////////////////////

#include "whc.h"


////////////////////////////////////////////////////////////
//
//	Set class attributes accordingly
//
////////////////////////////////////////////////////////////

WhcInternal::WhcInternal(int argc,char** argv){

	// query OpenCL compatible devices
	cl_uint numPlatforms;
	cl_platform_id platform[MAX_PLATFORMS];
	cl_device_id devices[MAX_DEVICES];
	cl_platform_id platformOfDevices[MAX_DEVICES];
	cl_uint numDevices=0;		
	cl_uint platformDevicesNum=0; 	
	
	// retain current state	
	void* current=NULL;
	cl_uint deviceID;

	//////////////////////////////////////////////////////////////
	// retrieve OpenCL platforms
	check( clGetPlatformIDs( MAX_PLATFORMS, platform, &numPlatforms ));
	if(numPlatforms==0)
		exit(-1);
	
	// loop through OpenCL platforms
	for( cl_uint i=0; i<numPlatforms; i++ ){
		check( clGetDeviceIDs( platform[i], CL_DEVICE_TYPE_ALL, MAX_DEVICES, devices + numDevices, &platformDevicesNum ));

		// setup platform list according to device
		for(cl_uint j=numDevices; j<platformDevicesNum; j++)
			platformOfDevices[j] = platform[i];

		// increase device count
		numDevices += platformDevicesNum;
		
	}

	//////////////////////////////////////////////////////////////
	// handle arguments given at runtime

	// reading input arguments
	for(int i=1; i<argc; i++){
		
		// lookup for input arguments
		if(!strcmp(argv[i], IN_DELIM))
			current = &inputList;
			
		// lookup for output arguments
		else if(!strcmp(argv[i], OUT_DELIM))
			current = &outputList;

		// lookup for optional arguments
		else if(!strcmp(argv[i], ARGS_DELIM))
			current = &argsList;

		else if(!strcmp(argv[i], DEV_DELIM))
			current = &deviceList;

		// push back arguments
		else if(current != (&deviceList))
				((vector<string>*)current)->push_back(argv[i]);
			else {
				deviceID = atoi(argv[i]);
				if((deviceID < numDevices) && (deviceID >= 0)){
					((vector<cl_device_id>*)current)->push_back( devices[deviceID] );
				}
			}
	}
}



/////////////////////////////////////////////////////////////////////
//
//	Error codes
//
////////////////////////////////////////////////////////////////////////

const char* print_cl_errstring(cl_int err) {
    switch (err) {
        case CL_SUCCESS:                          return  "Success!";
        case CL_DEVICE_NOT_FOUND:                 return  "Device not found."; 
        case CL_DEVICE_NOT_AVAILABLE:             return  "Device not available"; 
        case CL_COMPILER_NOT_AVAILABLE:           return  "Compiler not available"; 
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return  "Memory object allocation failure"; 
        case CL_OUT_OF_RESOURCES:                 return  "Out of resources";
        case CL_OUT_OF_HOST_MEMORY:               return  "Out of host memory";
        case CL_PROFILING_INFO_NOT_AVAILABLE:     return  "Profiling information not available";
        case CL_MEM_COPY_OVERLAP:                 return  "Memory copy overlap";
        case CL_IMAGE_FORMAT_MISMATCH:            return  "Image format mismatch";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return  "Image format not supported";
        case CL_BUILD_PROGRAM_FAILURE:            return  "Program build failure";
        case CL_MAP_FAILURE:                      return  "Map failure";
        case CL_INVALID_VALUE:                    return  "Invalid value";
        case CL_INVALID_DEVICE_TYPE:              return  "Invalid device type";
        case CL_INVALID_PLATFORM:                 return  "Invalid platform";
        case CL_INVALID_DEVICE:                   return  "Invalid device";
        case CL_INVALID_CONTEXT:                  return  "Invalid context";
        case CL_INVALID_QUEUE_PROPERTIES:         return  "Invalid queue properties";
        case CL_INVALID_COMMAND_QUEUE:            return  "Invalid command queue";
        case CL_INVALID_HOST_PTR:                 return  "Invalid host pointer";
        case CL_INVALID_MEM_OBJECT:               return  "Invalid memory object";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return  "Invalid image format descriptor";
        case CL_INVALID_IMAGE_SIZE:               return  "Invalid image size";
        case CL_INVALID_SAMPLER:                  return  "Invalid sampler";
        case CL_INVALID_BINARY:                   return  "Invalid binary";
        case CL_INVALID_BUILD_OPTIONS:            return  "Invalid build options";
        case CL_INVALID_PROGRAM:                  return  "Invalid program";
        case CL_INVALID_PROGRAM_EXECUTABLE:       return  "Invalid program executable";
        case CL_INVALID_KERNEL_NAME:              return  "Invalid kernel name";
        case CL_INVALID_KERNEL_DEFINITION:        return  "Invalid kernel definition";
        case CL_INVALID_KERNEL:                   return  "Invalid kernel";
        case CL_INVALID_ARG_INDEX:                return  "Invalid argument index";
        case CL_INVALID_ARG_VALUE:                return  "Invalid argument value";
        case CL_INVALID_ARG_SIZE:                 return  "Invalid argument size";
        case CL_INVALID_KERNEL_ARGS:              return  "Invalid kernel arguments";
        case CL_INVALID_WORK_DIMENSION:           return  "Invalid work dimension";
        case CL_INVALID_WORK_GROUP_SIZE:          return  "Invalid work group size";
        case CL_INVALID_WORK_ITEM_SIZE:           return  "Invalid work item size";
        case CL_INVALID_GLOBAL_OFFSET:            return  "Invalid global offset";
        case CL_INVALID_EVENT_WAIT_LIST:          return  "Invalid event wait list";
        case CL_INVALID_EVENT:                    return  "Invalid event";
        case CL_INVALID_OPERATION:                return  "Invalid operation";
        case CL_INVALID_GL_OBJECT:                return  "Invalid OpenGL object";
        case CL_INVALID_BUFFER_SIZE:              return  "Invalid buffer size";
        case CL_INVALID_MIP_LEVEL:                return  "Invalid mip-map level";
        default:                                  return  "Unknown";
    }
}

///////////////////////////////////////////////////////
//
//	Check function
//
///////////////////////////////////////////////////////

int check(int cuerr){

     if(cuerr!=CL_SUCCESS){
		printf("\n%s\n",print_cl_errstring(cuerr));
		return -1;
     	}

	return 0;
}

int check(int cuerr, cl_program program, cl_device_id device){

     if(cuerr!=CL_SUCCESS)
	{
		printf("\n%s\n",print_cl_errstring(cuerr));
		errorLog(program,device);
		return 1;
     	}

	return 0;
}

///////////////////////////////////////////////////////
//
//	Output log function
//
///////////////////////////////////////////////////////

string WhcInternal::outputLog(){

	string outLog;
	cl_device_type deviceType;
	cl_uint deviceCoreCount;
	char tempStr[MAX_STR_SIZE];
	char deviceVendor[MAX_STR_SIZE];
	char deviceName[MAX_STR_SIZE];
	char platformName[MAX_STR_SIZE];
	char platformVersion[MAX_STR_SIZE];

	// print argument info	
	outLog += "\nInput args: ";
	for(cl_uint i=0; i<inputList.size(); i++)
		outLog += inputList[i];

	outLog += "\nOutput args: ";
	for(cl_uint i=0; i<outputList.size(); i++)
		outLog += outputList[i];

	outLog += "\nAdditional args: ";
	for(cl_uint i=0; i<argsList.size(); i++)
		outLog += argsList[i];

	// print device(s) info
	sprintf(tempStr, "\nAllocated devices %d\n",(int)deviceList.size());
	outLog += tempStr;

	for(cl_uint i=0; i<deviceList.size(); i++){

			// get platform info

			cl_platform_id devicePlatform=NULL;
			check( clGetDeviceInfo( deviceList[i], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &devicePlatform, NULL ));
			check( clGetPlatformInfo(devicePlatform, CL_PLATFORM_VENDOR, sizeof(char)*MAX_STR_SIZE, platformName, NULL));
			check( clGetPlatformInfo(devicePlatform, CL_PLATFORM_VERSION, sizeof(char)*MAX_STR_SIZE, platformVersion, NULL));

			// get device info
			check( clGetDeviceInfo( deviceList[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &deviceCoreCount, NULL ));
			check( clGetDeviceInfo( deviceList[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &deviceType, NULL ));
			check( clGetDeviceInfo( deviceList[i], CL_DEVICE_VENDOR, sizeof(char) * MAX_STR_SIZE, deviceVendor, NULL));
			check( clGetDeviceInfo( deviceList[i], CL_DEVICE_NAME, sizeof(char) * MAX_STR_SIZE, deviceName, NULL));

			// print out device info
			if(deviceType==CL_DEVICE_TYPE_CPU)
				sprintf(tempStr, "\n[ Device %d ] Cores %d, Type CPU, Name %s, Vendor %s\n",i,deviceCoreCount,deviceName,deviceVendor);
			else if(deviceType==CL_DEVICE_TYPE_GPU)	
				sprintf(tempStr, "\n[ Device %d ] Cores %d, Type GPU, Name %s, Vendor %s\n",i,deviceCoreCount,deviceName,deviceVendor);
			else
				sprintf(tempStr, "\n[ Device %d ] Cores %d, Type N/A, Name %s, Vendor %s\n",i,deviceCoreCount,deviceName,deviceVendor);

			// append device info
			outLog += tempStr;

			// print out platform info
			sprintf(tempStr,"[ Platform vendor %s, version %s ]\n",platformName, platformVersion);
			outLog += tempStr;	
	}
	
	return outLog;
}

/////////////////////////////////////////////////////////////////////
//
//			ERROR LOG
//
/////////////////////////////////////////////////////////////////////

void errorLog(cl_program program, cl_device_id device)
{
		char* build_log;
		size_t log_size;
		
		// First call to know the proper size
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		build_log = new char[log_size+1];

		// Second call to get the log
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
		build_log[log_size] = '\0';
		printf("%s",build_log);
}
