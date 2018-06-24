#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string> 
#include <fstream> 
#include <ctime>
#include <sys/types.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <unistd.h>
#else
#include <CL/cl.h>
#endif

const double G = 0.00066741;
const double timestep = 0.05;
#define MAX_SOURCE_SIZE (0x100000)

using namespace std;

void checkError(cl_int err, string msg) //To print the error and terminate the program if any error occurs during the opencl API calls
{
	if(err != CL_SUCCESS)
	{
		cout<<msg<<endl;
		exit(0);
	}
}

int main(int argc, char** argv)
{
	int n=128;
	
	double duration=10; //in seconds
	int h = duration/timestep; // h =number of iterations
	float eps = 0.05;

	int nthreads = 64;

	float* pos = (float*) malloc(n*sizeof(cl_float4));
	float* pos_updated = (float*) malloc(n*sizeof(cl_float4));
	float* vel = (float*) malloc(n*sizeof(cl_float4));

	cl_mem d_pos, d_pos_updated, d_vel, pblock;




	ifstream dataset;
	double m,x,y,z, vx,vy,vz;

	dataset.open("tab128.txt");
	int k = 0;

	while(dataset)
	{
		dataset >> m >> x >> y >> z >> vx >> vy >> vz ;
		pos[k*4+0] = m;
		pos[k*4+1] = x;
		pos[k*4+2] = y;
		pos[k*4+3] = z;

		vel[k*4+0] = vx;
		vel[k*4+1] = vy;
		vel[k*4+2] = vz;
		vel[k*4+3] = 0.0f;

		k++;
		
	}
	dataset.close();

	// Load the kernel
	FILE *fp;
	const char fileName[]="./n_body_kernel.cl";
	size_t source_size;
	char* source_str;
	cl_int i;

	fp = fopen(fileName, "r");
	if(!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*) malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	// Finish loading the kernels

	cl_int err;
    cl_device_id device;     
    cl_context context;       
    cl_command_queue commands;      
    cl_program program;       
    cl_kernel kernel;     
    cl_uint numPlatforms;

    err = clGetPlatformIDs(0, NULL, &numPlatforms);
	checkError(err, "Unable to find any openCL platforms");
	if(numPlatforms==0)
	{
		printf("No opencl platforms available \n");
		exit(0);
	}

	cl_platform_id Platform[numPlatforms];
	err = clGetPlatformIDs(numPlatforms, Platform, NULL);

	//Get a GPU
	for(i=0;i<numPlatforms;i++)
	{
		err = clGetDeviceIDs(Platform[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
		if(err == CL_SUCCESS)
			break;
	}
	if (device == NULL)
	{
		printf("Error in finding a GPU platform \n");
		exit(0);
	}  

	context = clCreateContext(0, 1, &device, NULL, NULL, &err);
	checkError(err, "Unable to create context for the GPU device");

	commands = clCreateCommandQueue(context, device, 0, &err); 
	checkError(err, "Unable to create a command queue for the context");

	program = clCreateProgramWithSource(context, 1, (const char **) & source_str, NULL, &err);
	checkError(err, "Unable to create program from source");

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	checkError(err, "Unable to build program");

	kernel = clCreateKernel(program, "nbody_kernel", &err);
	checkError(err, "Unable to create kernel");

	d_pos = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(cl_float4), NULL, &err);
	d_pos_updated = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(cl_float4), NULL, &err);
	d_vel = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(cl_float4), NULL, &err);
	pblock = clCreateBuffer(context, CL_MEM_READ_ONLY, nthreads*sizeof(cl_float4), NULL, &err );

	err = clSetKernelArg(kernel, 0, sizeof(float), &timestep);
	err = clSetKernelArg(kernel, 1, sizeof(float), &eps);
    err = clSetKernelArg(kernel, 4, n*sizeof(cl_float4), vel);
   // err = clSetKernelArg(kernel, 5, nthreads*sizeof(cl_float4), &d_workGroupSums);	


    size_t global = n;
    size_t local = nthreads;


    for(int i=1;i<=h;i++)
    {
    	err = clSetKernelArg(kernel, 2, n*sizeof(cl_float4), &pos);
    	err = clSetKernelArg(kernel, 3, n*sizeof(cl_float4), &pos_updated);

    	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
		checkError(err, "Error in enqueueing the kernel");    	

    	err = clEnqueueReadBuffer(commands,d_pos, CL_TRUE, 0, sizeof(cl_float4) * n, pos, 0, NULL, NULL);
    	checkError(err, "Error in copying pos from device to host");

    	err = clEnqueueReadBuffer(commands,d_pos_updated, CL_TRUE, 0, sizeof(cl_float4) * n, pos_updated, 0, NULL, NULL);
    	checkError(err, "Error in copying pos_updated from device to host");

    }

    for(int i=0;i<n;i++)
    {
    	cout<<"Body "<<i<<" : "<<pos_updated[i*4+0]<<" "<<pos_updated[i*4+1]<<" "<<pos_updated[i*4+2]<<" "<<pos_updated[i*4+3]<<endl;
    }

	free(source_str);
    free(pos);
    free(pos_updated);
    free(vel);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
    clReleaseMemObject(d_pos);
    clReleaseMemObject(d_pos_updated);
    clReleaseMemObject(d_vel);

    return 0;
}

