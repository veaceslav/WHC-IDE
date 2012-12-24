////////////////////////////////////////////////////////
//
//	Lupescu Grigore 
//	Polytechnic University of Bucharest
//	OpenCL Kernel Development Template
//
//	Kernel Samples
//
////////////////////////////////////////////////////////


#define LEVEL 3


//////////////////////////////////////////////////////////////
//
//	Bit Splicing
//
//////////////////////////////////////////////////////////////

__kernel void bitSplicing(__global int* input,__global int* output,int width,int height)
{

    uint x = get_global_id(0);
    uint y = get_global_id(1);

	int toDivide=128;
	output[x+y*width]=((input[x+y*width] & toDivide)!=0)?0:255;
}