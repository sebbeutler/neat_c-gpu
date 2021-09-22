__kernel
void vector_add(__global const int *A, __global const int *B, __global int *C, int w) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
 
    // Do the operation
    C[i*w+j] = A[i*w+j] + B[i*w+j];
}