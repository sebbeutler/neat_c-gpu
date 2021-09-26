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
global int s_rand_seed = 0;
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
    char neuronList[MAX_NEURONS];
    matrix_seq M_seq;
    matrix_mask M_mask;
    int inputSize;
    int outputSize;
} Substrate;

global Substrate population[POP_COUNT];

void initBasic(int id, global matrix_seq M_seq, global matrix_mask M_mask, int inputSize, int outputSize)
{
    Substrate s = { {0}, M_seq, M_mask, inputSize, outputSize };
    for (int i=0; i<inputSize+outputSize; i++)
    {
        s.neuronList[i] = 1;
    }
    for (int i=0; i < inputSize; i++)
    {
        for (int j=inputSize; j < inputSize+outputSize; j++)
        {
            M_mask[i + j*neuron_count] = 1;
            M_mask[j + i*neuron_count] = -1;
            //M_seq[i + j*neuron_count] = pcg32_doublerand(&rand_seed);
        }
    }
    population[id] = s;
}

void mutate_add_link(int id)
{
    int n1 = 0;
    int n2 = 0;
    while (n1 < population[id].inputSize
        || n2 < population[id].inputSize
        || population[id].M_mask[n1*MAX_NEURONS + n2] == -1
        || n1 == n2
        || !population[id].neuronList[n1]
        || !population[id].neuronList[n2])
    {
        s_request(&s_rand_seed);
        n1 = pcg32_doublerand(&rand_seed) * neuron_count;
        n2 = pcg32_doublerand(&rand_seed) * neuron_count;
        s_release(&s_rand_seed);
    }

    population[id].M_mask[n1*MAX_NEURONS + n2] = 1;
    population[id].M_mask[n2*MAX_NEURONS + n1] = -1;
    
    s_request(&s_rand_seed);
    population[id].M_seq[n1*MAX_NEURONS + n2] = pcg32_doublerand(&rand_seed);
    s_release(&s_rand_seed);
}

// TODO: fast number generator with atomic_inc and dist pgc
void mutate_link_shift(int id)
{
    int n1, n2;
    do 
    {
        s_request(&s_rand_seed);
        n1 = pcg32i_boundedrand_r(&rand_seed, MAX_NEURONS);
        n2 = pcg32i_boundedrand_r(&rand_seed, MAX_NEURONS);
        s_release(&s_rand_seed);
    } while (population[id].M_mask[n2 * MAX_NEURONS + n1] != 1);

    s_request(&s_rand_seed);
    population[id].M_seq[n2 * MAX_NEURONS + n1] = pcg32_doublerand(&rand_seed);
    s_release(&s_rand_seed);
}

#endif // !EVOLUTION_H