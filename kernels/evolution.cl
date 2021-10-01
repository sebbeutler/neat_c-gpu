#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "kernels/pcg.cl"

#define matrixB byte_t*
#define matrixI int*
#define matrixF float*
#define matrixD double*

#define matrix_seq matrixD
#define matrix_mask matrixB

typedef char byte_t;

#define POP_COUNT 10
#define MAX_NEURONS 10

global pcg32u_random_t rand_seed;
global int neuron_count;

void s_request(global int* s)
{
    while (atom_xchg(s, 1) != 0) 0;
}

void s_release(global int* s)
{
    atom_xchg(s, 0);
}

typedef struct Substrate
{
    byte_t* neurons;
    matrix_seq M_seq;
    matrix_mask M_mask;
    int inputSize;
    int outputSize;
} Substrate;

global Substrate population[POP_COUNT];

// TODO: Add mutation neuron history matrixB
void initBasic(int id, global matrix_seq M_seq, global matrix_mask M_mask, int inputSize, int outputSize, global matrixB ncount)
{
    Substrate s = { ncount, M_seq, M_mask, inputSize, outputSize };
    for (int i = 0; i < inputSize + outputSize; i++)
    {
        s.neurons[i] = 1;
    }
    for (int i=0; i < inputSize; i++)
    {
        for (int j = 0; j<MAX_NEURONS; j++)
        {
            M_mask[i*neuron_count + j] = -1;
        }
        for (int j = inputSize; j < inputSize + outputSize; j++)
        {
            M_mask[j*neuron_count + i] = 1;
            //M_seq[j*neuron_count + i] = pcg32_doublerand(&rand_seed);
        }
    }
    population[id] = s;
}

void mutate_add_link(int id)
{
    int n_in = 0;
    int n_out = 0;
    while (population[id].M_mask[n_out*MAX_NEURONS + n_in] == -1
        || n_in == n_out
        || !population[id].neurons[n_in]
        || !population[id].neurons[n_out])
    {
        n_in = pcg32_doublerand(&rand_seed) * neuron_count;
        n_out = pcg32_doublerand(&rand_seed) * neuron_count;
    }

    population[id].M_mask[n_out*MAX_NEURONS + n_in] = 1;
    population[id].M_mask[n_in*MAX_NEURONS + n_out] = -1;
    population[id].M_seq[n_out*MAX_NEURONS + n_in] = pcg32_doublerand(&rand_seed);
}

void mutate_link_shift(int id)
{
    int n1, n2;
    do 
    {
        n1 = pcg32u_boundedrand_r(&rand_seed, MAX_NEURONS);
        n2 = pcg32u_boundedrand_r(&rand_seed, MAX_NEURONS);
    } while (population[id].M_mask[n2 * MAX_NEURONS + n1] != 1);

    population[id].M_seq[n2 * MAX_NEURONS + n1] = pcg32_doublerand(&rand_seed);
}

void mutate_neuron_add(int id)
{
    int x, y;
    do
    {
        x = pcg32u_boundedrand_r(&rand_seed, MAX_NEURONS);
        y = pcg32u_boundedrand_r(&rand_seed, MAX_NEURONS);
    } while (population[id].M_mask[y * MAX_NEURONS + x] != 1);

    population[id].M_mask[y * MAX_NEURONS + x] = -1;

    

}

#endif // !EVOLUTION_H