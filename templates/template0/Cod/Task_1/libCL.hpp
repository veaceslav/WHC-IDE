////////////////////////////////////////////////////////
//
//	Lupescu Grigore 
//	Polytechnic University of Bucharest
//	OpenCL Kernel Development Template
//
//	CL Error Check
//
////////////////////////////////////////////////////////

#include "CL/cl.h"
#include "lib.hpp"

//////////////////////////////////////////////////
// Function bodies
//////////////////////////////////////////////////

// check for errors
int check(int cuerr);
int check(int cuerr,cl_program program,cl_device_id device);

// log of compile errors
void errorLog(cl_program program,cl_device_id device);

// print error CL codes string
char *print_cl_errstring(cl_int err);

