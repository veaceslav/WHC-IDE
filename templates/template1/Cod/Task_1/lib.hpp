////////////////////////////////////////////////////////
//
//	Lupescu Grigore 
//	Polytechnic University of Bucharest
//	OpenCL Kernel Development Template
//
//	I/O PGM files
//
////////////////////////////////////////////////////////


#include<fstream>
#include<stdlib.h>
#include<cstdlib>
#include<stdio.h>
#include<string.h>

using namespace std;

//////////////////////////////////////////////////
// Function bodies
//////////////////////////////////////////////////

// get source code from filename [ byte array ]
char* readCode(char* filename); 

// get int array from pgm/ppm image
int* input_image(char* filename,int* width,int* height); 

// output pgm/ppm image
void output_image(char* filename,int *image,int width,int height);



