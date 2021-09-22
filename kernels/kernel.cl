__kernel
void vector_add(__global const int *A, __global const int *B, __global int *C, int w) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int j = get_global_id(1);
    /*
    // Map the two 2D indices to a single linear, 1D index 
    int grid_width = get_num_groups(0) * get_local_size(0);"
    int index = index_y * grid_width + index_x;"

    // Map the two 2D group indices to a single linear, 1D group index 
    int result = get_group_id(1) * get_num_groups(0) + get_group_id(0);"

    // Write out the result
    out[index] = result;"
    */
    // Do the operation
    C[i*w+j] = A[i*w+j] + B[i*w+j];
}


__kernel
void euler1(__global int *nb)
{
    int i = get_global_id(0);
    
    for (int j = 1; j<= 20; j++)
    {
        if (i % j != 0)
            return;
    }

    if (i < *nb || *nb == 0)
        *nb = i;
}