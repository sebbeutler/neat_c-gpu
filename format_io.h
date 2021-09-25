#ifndef FORMAT_IO_H
#define FORMAT_IO_H

#include "utils.h"

void print_matrixB(matrixB M, int sizeW, int sizeH);
void print_matrixI(matrixI M, int sizeW, int sizeH);
void print_matrixF(matrixF M, int sizeW, int sizeH);
void print_matrixD(matrixD M, int sizeW, int sizeH);

void print_genome(matrix_seq seq_global, matrix_mask seq_mask, int sizeH, int sizeW);

#endif // !FORMAT_IO_H