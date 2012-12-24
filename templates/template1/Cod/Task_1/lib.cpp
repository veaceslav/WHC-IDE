////////////////////////////////////////////////////////
//
//	Lupescu Grigore 
//	Polytechnic University of Bucharest
//	OpenCL Kernel Development Template
//
//	I/O PGM files
//
////////////////////////////////////////////////////////

#include "lib.hpp"

//////////////////////////////////////////////////////
//
//	Read CL Source Code from file 
//
//////////////////////////////////////////////////////

// returns char* code from filename
// NULL if error

char* readCode(char* filename)
	{

	FILE* file;
	int size,size_c;
	char* code;
	
	// check filename
	if(filename==NULL)return NULL;	

	// fill file pointer
	file = fopen (filename,"r") ;

	// nothing to do if file does not exist
	if(file==NULL)return NULL;

	// get file size, allocate memory
	fseek(file, 0, SEEK_END);  
	size = ftell(file);
	code=(char*)malloc((size+1)*sizeof(char));
	if(code==NULL)return NULL;	

	// set file pointer to start
	fseek(file, 0, SEEK_SET);  
	size_c=fread (code,1,size,file); 
	if(size_c!=size)return NULL;
	code[size]='\0';
	// close file
	fclose(file);

	return code;
	}




//////////////////////////////////////////////////////
//
//	Output PGM/PPM image
// 	Raw PGM only
//
//////////////////////////////////////////////////////


void output_image(char* filename,int* image,int width,int height)
	{
	//------------------------------------------------------- HEADER
	// create/erase file
	ofstream file_out(filename);

	// raw PGM data format 
	file_out<<"P5\n";

	// commnet on PGM file
	file_out<<"#Lupescu Grigore @2010\n";

	// write image size
	file_out<<width<<" "<<height<<"\n";

	// write max grayscale value
	file_out<<255<<"\n";

	// finish writing header
	file_out.close();	
	
	//------------------------------------------------------- BODY
	// attempt to append file for writing
	FILE* fp=fopen(filename, "a+b");
	if(fp==NULL)
		return;

	//write data
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			fwrite((char*)&image[i*height+j],1,1,fp);
		
	// close file
	fclose(fp);
	}

//////////////////////////////////////////////////////
//
//	Input PGM/PPM image
// 	RAW PGM Only
//
//////////////////////////////////////////////////////

int* input_image(char* filename,int* width,int* height)
	{

	int i=0,j=0;
	int imgWidth,imgHeight;
	int* image;
	char* buffer=new char[100];
  	size_t result;

	// open raw PGM file
	FILE* fp=fopen(filename, "r+b");
	if(fp==NULL)
		{
			printf("Could not open in.pgm file! Aborting...");
			return NULL;		
		}

	// read file type
	fgets (buffer,5,fp);

	// check file type
	if(strcmp(buffer,"P5\n")!=0)
		{
			printf("Input file has to be Raw PGM! Aborting...");
			fclose(fp);
			return NULL;		
		}


	// get rid of comments
	do{
		fgets(buffer,100,fp);
	}while(buffer[0]=='#');
			

	// image dimensions
	*height=atoi((const char*)strtok(buffer," "));
	*width=atoi((const char*)strtok(buffer," "));
	
	// get max grayscale size & ignore it (consider 255 always)
	fgets(buffer,10,fp);
		
	 // allocate memory for image
	image=(int*)malloc(sizeof(int)*(*height)*(*width));
	
	// check mem allocation
	if(!image)
		{
			printf("Memory allocation failed \n");
			fclose(fp);
			return NULL;
		}

	
	// read file data into 1D array
	for(int i=0;i<(*width);i++)
		for(int j=0;j<(*height);j++)
			{
				size_t readConf=fread (buffer,sizeof(char),1,fp);
				if(readConf!=0)
					image[j+i*(*width)]=(unsigned char)buffer[0];
			}
	fclose(fp);
	return image;
	}

