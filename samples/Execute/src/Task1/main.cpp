#include "main.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <time.h>
#include <sstream>
#include <cstdlib>

///
/// Error codes
///
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

void errorLog(cl_program prog, cl_device_id device)
{
	char* build_log;
	size_t log_size;

	// First call 
	clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG,0, NULL,
    &log_size);

    //Second Call

    clGetProgramBuildInfo(prog,device, CL_PROGRAM_BUILD_LOG,
log_size,build_log, NULL);

    build_log[log_size]='\0';
    printf("%s",build_log);

}

int check(int cuerr, cl_program prog, cl_device_id device)
{
    if(cuerr != CL_SUCCESS)
    {
        printf("\n %s \n",print_cl_errstring(cuerr));
        errorLog(prog,device);
        return 1;
    }
    return 0; //Success
}

int check(int cuerr){

     if(cuerr != CL_SUCCESS)
    {
        printf("\n%s\n", print_cl_errstring(cuerr));
        return 1;
        }

    return 0;
}
char* readCode()
{
    ifstream fs("kernel.cl");
    string rez("");
    string tmp;
    if(fs.is_open())
    {
/**
        while(fs.good())
        {
            getline(fs,tmp);
            rez+=tmp;
            rez+="\n";
        }
*/
	stringstream buff;
	buff << fs.rdbuf();
	rez = buff.str();
        fs.close();
    }

    //cout << rez << endl;
    return strdup(rez.data());
}
int main (int argc, char** argv)
{

    if(argc < 7)
    {
        printf("Usage: ./whc -in in.txt -out out.txt -dev 1");
        return 1;
    }

    fstream fs(argv[2]);
    printf("%s",argv[2]);
    int inputSize =0;

    char tempString[MAX_STR_SIZE];

    cl_platform_id platform[MAX_PLATFORMS]; // Platforme disponibile

    cl_device_id device;
    cl_device_id devices[MAX_DEVICE_COUNT]; // Lista de device-ri per platforma

    cl_uint totalDevices=0;
    cl_uint platformDevices=0;
    cl_uint currentDevice=0;

    cl_context context;
    cl_command_queue queue;
    cl_int err;
    cl_program program;
    cl_kernel kernel;

    cl_mem bufferIn;
    cl_mem bufferOut;

    float* inputData;
    float* outputData;

    size_t global_work_size;

    cl_uint num_platforms;

    // gasim platforme
      // readCode();
    clGetPlatformIDs(MAX_PLATFORMS,platform,&num_platforms);

    if(num_platforms == 0)
    {
        printf("No platform found \n");
        exit(-1);
    }
    else
        printf("Found %d platforms \n\n", num_platforms);


    for(int i=0;i<num_platforms;i++)
    {

        clGetPlatformInfo(platform[i],CL_PLATFORM_NAME, sizeof(char)*MAX_STR_SIZE,tempString,NULL);
            printf("[%c] %s \n",(char)(i+(int)'A'),tempString);

        clGetPlatformInfo(platform[i],CL_PLATFORM_VENDOR,sizeof(char)*MAX_STR_SIZE,tempString,NULL);
            printf("- Plaform vendor: %s", tempString);

        clGetPlatformInfo(platform[i],CL_PLATFORM_VERSION,sizeof(char)*MAX_STR_SIZE,tempString,NULL);
            printf("- Platform Version: %s",tempString);


        clGetDeviceIDs(platform[i],CL_DEVICE_TYPE_ALL,MAX_DEVICE_COUNT,devices+
                        totalDevices,&platformDevices);
            totalDevices+=platformDevices;

            printf("-Devices available : %d\n\n",platformDevices);
    }

    for(int j=0;j<totalDevices;j++)
    {
        clGetDeviceInfo(devices[j],CL_DEVICE_NAME,sizeof(char)*MAX_STR_SIZE,tempString,NULL);
            printf("[%d]%s\n",j,tempString);
    }

    printf("\n Select device to run on:");

    fflush(stdout);
    
    //scanf("%d",&currentDevice);

    currentDevice=atoi(argv[6]);
    //currentDevice = 0;

    printf("Current device %d\n",currentDevice);

    // verificam daca device-ul selectat exista
    if(currentDevice < 0 || currentDevice > totalDevices)
    {
        printf("Selected device doesn't exist. Exiting ... ");
        exit(-1);
    }
    else
        printf("Continue ...");

    clGetDeviceInfo(devices[currentDevice],CL_DEVICE_NAME,sizeof(char)*MAX_STR_SIZE,tempString,NULL);
    
    device = devices[currentDevice];

    printf("Running on device %s \n", tempString);


        // create context & add 1 queue for 1 device
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue   = clCreateCommandQueue(context, device, 0, NULL);

    fs >> inputSize;
    //inputSize = 10;
    
    inputData = new float[inputSize];
    outputData = new float[inputSize];

    if((!inputData) || (!outputData))
    {
        printf("Eroare la alocare pe heap\n");
        exit(-1);
    }
    // de la 0 la 15
        
    for(int i=0;i<inputSize;i++)
    {
        fs >> inputData[i];
        //printf("%f \n",inputData[i]);
    }
    // alocam memorie pentru OpenCL

        //printf("Checkpoint \n");
        bufferIn = clCreateBuffer(context, CL_MEM_READ_ONLY, inputSize*sizeof(float),NULL,NULL);

        bufferOut = clCreateBuffer(context,CL_MEM_WRITE_ONLY, inputSize*sizeof(float),NULL,NULL);

        // scriem datele in bufferul de intrare
        //printf("Checkpoint 2\n");
        clEnqueueWriteBuffer(queue,bufferIn,1,0, inputSize*sizeof(float),inputData,0,0,0);

        /**
        const char* source =
        "//OpenCL kernel code, c99 derived \n"
        "__kernel void doubleInput(__global float* input,__global float* output) \n"
        "{ \n"
        "int tid = get_global_id(0); \n"
        "output[tid] = 2*input[tid]; \n"
        "} \n"
        "\n\0";
    */

        const char* source = readCode();
                //printf("%s",source);
    // build OenCl program

        //printf("am scris programul pentru kernel\n");
        program = clCreateProgramWithSource(context, 1, &source, NULL, &err);
        check(err,program,device);
        //printf("Pass");
        check(clBuildProgram(program, 1, &device, NULL, NULL, NULL));
        //printf("Pass 2");

            // kernels
        kernel = clCreateKernel(program, "doubleInput", &err);
        check(err, program, device);
        
        //printf("Compilat kernelul\n");
        global_work_size = inputSize;

        check(clSetKernelArg(kernel,0,sizeof(bufferIn),(void*)&bufferIn));
        check(clSetKernelArg(kernel,1,sizeof(bufferOut),(void*)&bufferOut));

        size_t global_work = inputSize;

        clock_t start,end;

        //start = clock();
        //printf("Executam\n");
        check(clEnqueueNDRangeKernel(queue, kernel,1,NULL,&global_work,NULL,0,NULL,NULL));
        //printf("Gata!");
        check(clFinish(queue));
        //end = clock();

        //cout << "Timp scurs" << (double)(end-start)/CLOCKS_PER_SEC << endl;

        clEnqueueReadBuffer(queue,bufferOut,CL_TRUE,0,inputSize*sizeof(float),outputData,0,0,0);

        ofstream os(argv[4]);
        os << inputSize << endl;
        for(int i=0;i<inputSize;i++)
        {
            cout << outputData[i] << endl;
            os << outputData[i] << " ";
        }

        return 0;
}
