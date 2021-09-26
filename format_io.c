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

void print_genome(matrix_seq seq_global, matrix_mask seq_mask, int sizeH, int sizeW)
{
    printf("      | ");
    for (int i=0; i < 3; i++)
    {
        for (int j=0; j < sizeW; j++)
        {
            printf("  %02d  |", j+1);
        }
        printf("\t|");
    }
    printf("\n");

    printf("------");
    for (int i=0; i < 3; i++)
    {
        for (int j=0; j < sizeW; j++)
        {
            printf("-------");
        }
        printf("\t");
    }
    printf("\n");
    for (int i=0; i < sizeH; i++)
    {
        printf("  %02d  |", i+1);

        for (int j=0; j < sizeW; j++)
        {
            printf(" %.2lf |", seq_global[i*sizeH + j]);
        }
        printf("\t|");
        for (int j=0; j < sizeW; j++)
        {
            printf("  %02d  |", seq_mask[i*sizeH + j]);
        }
        printf("\t|");
        for (int j=0; j < sizeW; j++)
        {
            if (seq_mask[i*sizeH + j] == 1)
            {
                printf(" %.2lf |", seq_global[i*sizeH + j]);
            }
            else
            {
                printf("      |");
            }
        }
        printf("\n");
    }
    printf("\n");
}