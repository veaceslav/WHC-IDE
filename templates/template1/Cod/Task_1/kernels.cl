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

///////////////////////////////////////////////////////
//
//	dilation
//
///////////////////////////////////////////////////////

__kernel void dilate(__global int* input,__global int* output,int width,int height)
{

    	uint x = get_global_id(0);
    	uint y = get_global_id(1);
	
	// frame width
	int frameW=LEVEL;	
	// frame array
	int frameA=LEVEL*LEVEL;		

	int frameLeft=x;
	int frameBottom=y;

	int frameTop=((x+frameW) < (width)) ? (x+frameW):(width);
	int frameRight=((y+frameW) < (height)) ? (y+frameW):(height);

	// get center value (1/0)
	int center=input[(frameLeft+1)+width*(frameBottom+1)]/255;		
	
	// dilate
	output[(frameLeft+1)+width*(frameBottom+1)]=center*255;	// center (1,1)

	output[(frameLeft+0)+width*(frameBottom+1)]=((input[(frameLeft+0)+width*(frameBottom+1)] /255) | center )*255; // up (0,1)
	output[(frameLeft+1)+width*(frameBottom+2)]=((input[(frameLeft+1)+width*(frameBottom+2)] /255) | center )*255; // down (2,1)
	output[(frameLeft+1)+width*(frameBottom+0)]=((input[(frameLeft+1)+width*(frameBottom+0)] /255) | center )*255; // left (1,0)
	output[(frameLeft+2)+width*(frameBottom+1)]=((input[(frameLeft+2)+width*(frameBottom+1)] /255) | center )*255; // right (1,2)
}

///////////////////////////////////////////////////////
//
//	erosion
//
///////////////////////////////////////////////////////

__kernel void erosion(__global int* input,__global int* output,int width,int height)
{

    	uint x = get_global_id(0);
    	uint y = get_global_id(1);

	// frame width
	int frameW=LEVEL;	
	// frame array
	int frameA=LEVEL*LEVEL;		

	int frameLeft=x;
	int frameBottom=y;

	int frameTop=((x+frameW) < (width)) ? (x+frameW):(width);
	int frameRight=((y+frameW) < (height)) ? (y+frameW):(height);

	// get center value (1/0)
	int center=input[(frameLeft+1)+width*(frameBottom+1)]/255;		
	
	// dilate
	output[(frameLeft+1)+width*(frameBottom+1)]=center*255;	// center (1,1)

	output[(frameLeft+0)+width*(frameBottom+1)]=((input[(frameLeft+0)+width*(frameBottom+1)] /255) & center )*255; // up (0,1)
	output[(frameLeft+1)+width*(frameBottom+2)]=((input[(frameLeft+1)+width*(frameBottom+2)] /255) & center )*255; // down (2,1)
	output[(frameLeft+1)+width*(frameBottom+0)]=((input[(frameLeft+1)+width*(frameBottom+0)] /255) & center )*255; // left (1,0)
	output[(frameLeft+2)+width*(frameBottom+1)]=((input[(frameLeft+2)+width*(frameBottom+1)] /255) & center )*255; // right (1,2)
}


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
