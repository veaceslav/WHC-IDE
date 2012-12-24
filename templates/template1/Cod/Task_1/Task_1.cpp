////////////////////////////////////////////////////////
//
//	Lupescu Grigore 
//	Polytechnic University of Bucharest
//	OpenCL Kernel Development Template
//
//	MAIN Function & Settings
//
////////////////////////////////////////////////////////

#include "lib.hpp"
#include "libCL.hpp"


////////////////////////////////////////////////////////
// I/O Files

#define INPUT_IMAGE "in.pgm"
#define OUTPUT_IMAGE "out.pgm"
#define FILENAME "kernels.cl"
#define NWITEMS 256

////////////////////////////////////////////////////////
// Pipeline kernels

#define COMPUTE CL_DEVICE_TYPE_CPU
#define NR_KERNELS 3

char kernels[NR_KERNELS][15]={"bitSplicing", "erosion", "dilate"};

////////////////////////////////////////////////////////

/////////////////////////////////////////////////
//
//		MAIN FUNCTION
//
/////////////////////////////////////////////////

int main(int argc,char** argv)
	{
	int width;
	int height;
	
	const char* source;		// Source code
	cl_platform_id platform;	// Platform { CPU, GPU, FPGA }
	cl_device_id device;		// Device { GPU_ID }
	cl_context context;		// Context
	cl_command_queue queue;		// Queue 
	cl_int err;			// for error checking

	cl_event event[NR_KERNELS-1];	// must be ok for second kernel to run

	cl_program program;		// program
	cl_kernel kernels_code[NR_KERNELS];


	cl_mem bufferIN;	// data IN
	cl_mem bufferOUT;	// data OUT

	int* inputImage;	// input image
	int* outputImage;	// output image

	cl_uint *ptr;			// data to be shown
	size_t global_work_size;	// work size

	// get platform, get device, create context
	clGetPlatformIDs(1,&platform,NULL);
	clGetDeviceIDs(platform,COMPUTE,1,&device,NULL);
	context=clCreateContext(NULL,1,&device,NULL,NULL,NULL);
	queue=clCreateCommandQueue(context,device,0,NULL);

	
	
	// read data from file, allocate memory & copy
	inputImage=input_image(INPUT_IMAGE,&width,&height);
	
	bufferIN=clCreateBuffer(context,CL_MEM_READ_WRITE,width*height *sizeof(int),NULL,NULL);
	clEnqueueWriteBuffer(queue,bufferIN,1,0,width * height * sizeof(int),inputImage,0,0,0);

	bufferOUT= clCreateBuffer(context,CL_MEM_READ_WRITE,width*height *sizeof(int),NULL,NULL);	

	// read code from file / check code
	source=readCode(FILENAME);
	if(source==NULL)
		{
		printf("Error reading code. Exiting");
		return 1;
		}
	
	// init OPENCL program
	
	
	program=clCreateProgramWithSource(context,1,&source,NULL,&err);
	check(err,program,device);
	check(clBuildProgram(program,1,&device,NULL,NULL,NULL));


	// kernels
	for(int i=0;i<NR_KERNELS;i++)
		{
			kernels_code[i]=clCreateKernel(program,kernels[i],&err);
			check(err,program,device);
		}
	

	// pass args, execute OPENCL kernels
	global_work_size=width*height;

	for(int i=0;i<NR_KERNELS;i++)
	{	
		if((i+1)%2==0){
			check(clSetKernelArg(kernels_code[i],0,sizeof(bufferOUT),(void*)&bufferOUT));
			check(clSetKernelArg(kernels_code[i],1,sizeof(bufferIN),(void*)&bufferIN));
			check(clSetKernelArg(kernels_code[i],2,sizeof(int),(void*)&width));
			check(clSetKernelArg(kernels_code[i],3,sizeof(int),(void*)&height));
			}
		else {
			check(clSetKernelArg(kernels_code[i],0,sizeof(bufferIN),(void*)&bufferIN));
			check(clSetKernelArg(kernels_code[i],1,sizeof(bufferOUT),(void*)&bufferOUT));
			check(clSetKernelArg(kernels_code[i],2,sizeof(int),(void*)&width));
			check(clSetKernelArg(kernels_code[i],3,sizeof(int),(void*)&height));
			}
	}

	

	
	size_t global_work[2];
	global_work[0]=width;
	global_work[1]=height;

	// enqueue / run all kernels (pipeline in this case)

	// start first kernel
	check(clEnqueueNDRangeKernel(queue,kernels_code[0],2,NULL,global_work,NULL,0,NULL,&event[0]));

	// run all the rest (n-2)
	for(int i=1;i<NR_KERNELS-1;i++)
		check(clEnqueueNDRangeKernel(queue,kernels_code[i],2,NULL,global_work,NULL,i,event,&event[i]));

	// start last kernel
	if(NR_KERNELS>2)
	check(clEnqueueNDRangeKernel(queue,kernels_code[NR_KERNELS-1],2,NULL,global_work,NULL,NR_KERNELS-1,event,NULL));
		
	
	// finish queue
	check(clFinish(queue));
	 
	if(NR_KERNELS%2==1)
    		outputImage= (int*)clEnqueueMapBuffer(queue,bufferOUT,CL_TRUE,CL_MAP_READ,0,width*height*sizeof(int),0,NULL,NULL,NULL);
	else 
		outputImage= (int*)clEnqueueMapBuffer(queue,bufferIN,CL_TRUE,CL_MAP_READ,0,width*height*sizeof(int),0,NULL,NULL,NULL);
    	
    	output_image(OUTPUT_IMAGE,outputImage,width,height);
	return 0;
}