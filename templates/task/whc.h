//////////////////////////////////////////////////////////////
//
//	WHC template header. July 2011
//	Version 0.1
//
//////////////////////////////////////////////////////////////

#ifndef _WHC_INTERNAL
#define _WHC_INTERNAL

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "iostream"
#include "vector"

#include "CL/cl.h"

//////////////////////////////////
//
//	ARGS Delimitors
//
//////////////////////////////////

#define IN_DELIM	"-in"
#define OUT_DELIM	"-out"
#define ARGS_DELIM	"-args"
#define DEV_DELIM	"-dev"

#define MAX_PLATFORMS 5
#define MAX_STR_SIZE 256
#define MAX_DEVICES 10

using namespace std;


///////////////////////////////////////////////
//
//	Internal structure definitions
//
///////////////////////////////////////////////

class WhcInternal{

	public:
		WhcInternal(int argc,char** argv);
		string outputLog();

		vector<string> inputList;
		vector<string> outputList;
		vector<string> argsList;
		vector<cl_device_id> deviceList;
	
};

///////////////////////////////////////////////
//
//	Functions for printing out errors
//
///////////////////////////////////////////////

int check(int cuerr);
int check(int cuerr, cl_program program, cl_device_id device);
const char* print_cl_errstring(cl_int err);
void errorLog(cl_program program, cl_device_id device);

#endif

