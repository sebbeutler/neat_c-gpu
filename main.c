#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <CL/cl.h>

#include "utils.h"
#include "format_io.h"

cl_program load_program(cl_context context, char* filename, cl_int* errcode_ret);

char BIN_PATH[255];
int main(int argc, char *argv[])
{
    strcpy(BIN_PATH, argv[0]);
    for (int i = strlen(BIN_PATH) - 1; i >= 0; i--)
    {
        if (BIN_PATH[i] == '/' || BIN_PATH[i] == '\\')
        {
            BIN_PATH[i + 1] = '\0';
            break;
        }
    }

#pragma region OPENCL_INIT

    cl_int ret = 0;
    size_t global_item_size[3];
    size_t local_item_size[3];
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    ret |= clGetPlatformIDs(1, &platform_id, NULL);
    ret |= clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

#pragma endregion OPENCL_INIT

#pragma region KERNEL_EXEC

    cl_program program = load_program(context, "kernels/kernels.cl", &ret);
    ret |= clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    cl_kernel kernel;

    // Init buffers
    int sizeH = 10;
    int sizeW = 10;
    int pop_count = 10;
    size_t pop_size = pop_count * sizeW * sizeH * sizeof(double);
    size_t mask_size = pop_count * sizeW * sizeH * sizeof(byte_t);
    
    matrix_seq seq_global = malloc(pop_size); if (!seq_global) return -4;
    memset(seq_global, 0, pop_size);
    
    matrix_mask seq_mask = malloc(mask_size); if (!seq_mask) return -4;
    memset(seq_mask, 0, mask_size);

    cl_mem pop_mem =  clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR,
        pop_size,
        seq_global,
        &ret);

    cl_mem mask_mem =  clCreateBuffer(
        context,
        CL_MEM_COPY_HOST_PTR,
        mask_size,
        seq_global,
        &ret);
    
    // Run kernel > new_session
    kernel = clCreateKernel(program, "new_session", &ret);
    unsigned long initstate = (unsigned long)time(NULL);
    unsigned long initseq = (unsigned long)&printf;
    ret |= clSetKernelArg(kernel, 0, sizeof(unsigned long), &initstate);
    ret |= clSetKernelArg(kernel, 1, sizeof(unsigned long), &initseq);
    global_item_size[0] = 1;
    local_item_size[0] = 1;
    ret |= clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    clReleaseKernel(kernel);

    // Run kernel > evolve
    kernel = clCreateKernel(program, "evolve", &ret);

    ret |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &pop_mem);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &mask_mem);
    ret |= clSetKernelArg(kernel, 2, sizeof(int), &sizeW);
    ret |= clSetKernelArg(kernel, 3, sizeof(int), &sizeH);

    global_item_size[0] = 10;
    local_item_size[0] = 1;
    ret |= clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    
    
    // Read buffers
    ret |= clEnqueueReadBuffer(command_queue, pop_mem, CL_TRUE, 0, pop_size, seq_global, 0, NULL, NULL);
    ret |= clReleaseMemObject(pop_mem);
    ret |= clEnqueueReadBuffer(command_queue, mask_mem, CL_TRUE, 0, mask_size, seq_mask, 0, NULL, NULL);
    ret |= clReleaseMemObject(mask_mem);

    clReleaseKernel(kernel);


#pragma endregion KERNEL_EXEC

#pragma region KERNEL_POST_EXEC

    for (int i=0; i<pop_count; i++)
    {
        //print_genome(seq_global + i*sizeW*sizeH, seq_mask + i*sizeW*sizeH, sizeH, sizeW);
    }
    print_genome(seq_global, seq_mask, sizeH, sizeW);

#pragma endregion KERNEL_POST_EXEC


#pragma region OPENCL_CLEAN

    printf("OpenCL error: [%d]\n", ret);

    // clean
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);    
    clReleaseContext(context);

#pragma endregion OPENCL_CLEAN

    return 0;
}

cl_program load_program(cl_context context, char* filename, cl_int* errcode_ret)
{
    FILE *fp;
    char *source_str;
    size_t source_size;


    char kernel_path[255];
    strcpy(kernel_path, BIN_PATH);
    strcat(kernel_path, filename);
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    if (!source_str)
    {
        *errcode_ret |= -4;
        return 0;
    }
    fp = fopen(kernel_path, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    return clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, errcode_ret);
}

