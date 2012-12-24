#ifndef MAIN_H
#define MAIN_H

#include <CL/cl.h>
#include <iostream>
#include <stdio.h>

using namespace std;

//Application setting
#define MAX_STR_SIZE	512
#define MAX_PLATFORMS	8
#define MAX_DEVICE_COUNT	16
#define NR_ELEMENTS		100000

const char* print_cl_error(cl_int err);

void errorLog(cl_program prog, cl_device_id device);

int check(int cuerr, cl_program prog, cl_device_id device);

int check (int cuerr);


#endif
