#include "kernels/evolution.cl"
#include "kernels/pcg.cl"


kernel void new_session(uint initstate, uint initseq)
{
    neuron_count = 10;
	pcg32_srandom_r(&rand_seed, initstate, initseq);
}

kernel void evolve(global matrix_seq seq_global, global matrix_mask seq_mask, int sizeW, int sizeH)
{
    local int i;

    int id = get_global_id(0);

    initBasic(id,
    seq_global + id * sizeW * sizeH, 
    seq_mask + id * sizeW * sizeH, 
    3, 2);

    mutate_link_shift(id);

}