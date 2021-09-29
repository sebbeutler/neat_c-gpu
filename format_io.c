#include "format_io.h"

#include <stdio.h>

void print_matrixB(matrixB M, int sizeW, int sizeH)
{
    for (int i=0; i < sizeH; i++)
    {
        for (int j=0; j< sizeW; j++)
        {
            printf("%d    ", M[i*sizeH + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrixI(matrixI M, int sizeW, int sizeH)
{
    for (int i=0; i < sizeH; i++)
    {
        for (int j=0; j< sizeW; j++)
        {
            printf("%d    ", M[i*sizeH + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrixF(matrixF M, int sizeW, int sizeH)
{
    for (int i=0; i < sizeH; i++)
    {
        for (int j=0; j< sizeW; j++)
        {
            printf("%.2f ", M[i*sizeH + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrixD(matrixD M, int sizeW, int sizeH)
{
    for (int i=0; i < sizeH; i++)
    {
        for (int j=0; j< sizeW; j++)
        {
            printf("%.2lf ", M[i*sizeH + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_genome(matrix_seq seq_global, matrix_mask seq_mask, int size, matrixB ncount)
{
    printf("      | ");
    for (int i=0; i < 3; i++)
    {
        for (int j=0; j < size; j++)
        {
            printf("  %02d  |", j+1);
        }
        printf("\t|");
    }
    printf("\n");

    printf("------");
    for (int i=0; i < 3; i++)
    {
        for (int j=0; j < size; j++)
        {
            printf("-------");
        }
        printf("\t");
    }
    printf("\n");
    for (int i=0; i < size; i++)
    {
        printf("  %02d  |", i+1);

        for (int j=0; j < size; j++)
        {
            if (seq_global[i*size + j] != 0)
            {
                printf("\033[0;32m %.2lf \033[0m|", seq_global[i*size + j]);
            }
            else
            {
                printf(" %.2lf |", seq_global[i*size + j]);
            }
        }
        printf("\t|");
        for (int j=0; j < size; j++)
        {
            if (seq_mask[i*size + j] == 1)
            {
                printf("\033[0;36m  01  \033[0m|");
            }
            else if (seq_mask[i*size + j] == -1)
            {
                printf("\033[0;31m  -1  \033[0m|");
            }
            else
            {
                printf("  %02d  |", seq_mask[i*size + j]);
            }
        }
        printf("\t|");
        for (int j=0; j < size; j++)
        {
            if (seq_mask[i*size + j] == 1)
            {
                if (seq_global[i*size + j] != 0)
                    printf("\033[0;31m %.2lf \033[0m|", seq_global[i*size + j]);
                else
                    printf("\033[0;33m %.2lf \033[0m|", seq_global[i*size + j]);
            }
            else
            {
                printf("      |");
            }
        }
        printf("\n");
    }
    printf("\033[0;34m");
    printf("Neurons : ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", ncount[i]);
    }
    printf("\033[0m");
    printf("\n");
    printf("\n");
}