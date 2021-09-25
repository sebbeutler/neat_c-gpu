#define matrixB byte_t*
#define matrixI int*
#define matrixF float*
#define matrixD double*

#define matrix_seq matrixD
#define matrix_mask matrixB

typedef char byte_t;

global int neuron_count;
kernel void set_neuron_count(int n) { neuron_count = n; }

void GetSemaphor(__global int* semaphor) {
   int occupied = atom_dec(semaphor);
   while(occupied > 0)
   {
     occupied = atom_dec(semaphor);
   }
}

void ReleaseSemaphor(__global int* semaphor)
{
   int prevVal = atom_xchg(semaphor, 0);
}

void initBasic(global matrix_mask M, int inputSize, int outputSize)
{
    for (int i=0; i < inputSize; i++)
    {
        for (int j=inputSize; j < inputSize+outputSize; j++)
        {
            M[i + j*10] = 1;
        }
    }
}

kernel void evolve(global matrix_seq seq_global, global matrix_mask seq_mask)
{
    local int i;

    int id = get_global_id(0);
    int y = get_global_id(1);
    int x = get_global_id(2);
    int size = get_global_size(0);
    int seq_pos = id * get_global_id(1) * get_global_id(2) + y * get_global_id(1) + x;
    
    if (atom_xchg(&i, 1) != 1)
        initBasic(seq_mask + id*size*size, 3, 2);
}




