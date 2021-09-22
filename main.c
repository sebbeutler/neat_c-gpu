#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

cl_program load_program(cl_context context, char* filename);

void print_mat(int *mat, int width, int height);

char BIN_PATH[255];
int main(int argc, char *argv[])
{
    strcpy(BIN_PATH, argv[0]);
    for (size_t i = strlen(BIN_PATH) - 1; i >= 0; i--)
    {
        if (BIN_PATH[i] == '/' || BIN_PATH[i] == '\\')
        {
            BIN_PATH[i + 1] = '\0';
            break;
        }
    }

    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    clGetPlatformIDs(1, &platform_id, NULL);
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, NULL);

/*
    const int MWIDTH = 10;
    const int MHEIGHT = 10;
    const size_t MSIZE = MWIDTH * MHEIGHT * sizeof(int);
    int A[10][10];
    int B[10][10];
    int C[10][10];
    for (int i = 0; i < MWIDTH; i++)
    {
        for (int j = 0; j < MHEIGHT; j++)
        {
            A[i][j] = 1;
            B[i][j] = 2;
        }
    }
*/

    cl_program program = load_program(context, "kernel.cl");
    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
/*
    cl_kernel kernel = clCreateKernel(program, "vector_add", NULL);

    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,  MSIZE, NULL, NULL);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,  MSIZE, NULL, NULL);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, MSIZE, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, MSIZE, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, MSIZE, B, 0, NULL, NULL);
    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &a_mem_obj);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &b_mem_obj);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &c_mem_obj);
    clSetKernelArg(kernel, 3, sizeof(int), &MWIDTH);

    size_t global_item_size[] = { MWIDTH, MHEIGHT };
    size_t local_item_size[] = { 1, 1 };
    clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, MSIZE, C, 0, NULL, NULL);
    
    print_mat((int*)A, MWIDTH, MHEIGHT);
    print_mat((int*)B, MWIDTH, MHEIGHT);
    print_mat((int*)C, MWIDTH, MHEIGHT);

    clReleaseMemObject(a_mem_obj);
    clReleaseMemObject(c_mem_obj);
*/
    
    int nb = CL_DEVICE_MAX_WORK_ITEM_SIZES ;
    cl_kernel kernel = clCreateKernel(program, "euler1", NULL);
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_obj);
    size_t global_item_size = 1000000000;
    size_t local_item_size = 1000;
    clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    clEnqueueReadBuffer(command_queue, mem_obj, CL_TRUE, 0, sizeof(int), &nb, 0, NULL, NULL);
    clReleaseMemObject(mem_obj);
    printf("NB: %d\n", nb);


    // clean
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseCommandQueue(command_queue);    
    clReleaseKernel(kernel);
    clReleaseProgram(program);    
    clReleaseContext(context);
    
    return 0;
}

cl_program load_program(cl_context context, char* filename)
{
    FILE *fp;
    char *source_str;
    size_t source_size;


    char kernel_path[255];
    strcpy(kernel_path, BIN_PATH);
    strcat(kernel_path, filename);
    fp = fopen(kernel_path, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    return clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, NULL);
}

void print_mat(int *mat, int width, int height)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            printf("%d ", *(mat + (i * sizeof(int)) + j));
        }
        printf("\n");
    }
    printf("\n");
}