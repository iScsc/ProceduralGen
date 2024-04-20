#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#include "gradientGrid.h"

void setRandomSeed(unsigned int seed)
{
    srand(seed);
}

gradientGrid* newGradGrid(int width, int height)
{
    gradientGrid* new_grad_grid = calloc(1, sizeof(gradientGrid));

    vector* gradients = calloc(width * height, sizeof(vector));

    new_grad_grid->width = width;
    new_grad_grid->height = height;
    new_grad_grid->gradients = gradients;

    return new_grad_grid;
}



gradientGrid* newRandomGradGrid(int width, int height)
{
    gradientGrid* new_grad_grid = newGradGrid(width, height);

    regenerateRandomGradGrid(new_grad_grid);

    return new_grad_grid;
}



void regenerateRandomGradGrid(gradientGrid* p_gradGrid)
{
    int width = p_gradGrid->width;
    int height = p_gradGrid->height;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = (p_gradGrid->gradients) + i * width + j;

            v->x = (double) rand()/RAND_MAX;
            v->y = sqrt(1. - v->x * v->x);
        }
    }
}



void printGradientGrid(gradientGrid* p_gradGrid)
{
    int width = p_gradGrid->width;
    int height = p_gradGrid->height;

    printf("-------------------------------------------\n");
    printf("Printing gradient grid of size = (%d, %d)\n\n", height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = (p_gradGrid->gradients) + i * width + j;

            printf("(%lf, %lf)   ", v->x, v->y);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeGradGrid(gradientGrid* p_gradGrid)
{
    free(p_gradGrid->gradients);
    free(p_gradGrid);
}
