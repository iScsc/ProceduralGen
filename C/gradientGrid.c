#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#include "loadingBar.h"
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



gradientGrid* newRandomGradGrid(int width, int height, int display_loading)
{
    gradientGrid* new_grad_grid = newGradGrid(width, height);

    regenerateRandomGradGrid(new_grad_grid, display_loading);

    return new_grad_grid;
}



void regenerateRandomGradGrid(gradientGrid* p_gradGrid, int display_loading)
{
    int width = p_gradGrid->width;
    int height = p_gradGrid->height;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = getVector(p_gradGrid, j, i); //(p_gradGrid->gradients) + i * width + j;

            v->x = (double) rand()/RAND_MAX;
            v->y = sqrt(1. - v->x * v->x);

            if (display_loading != 0)
            {
                // max :  (width - 1) + (height - 1) * width  =  width * height - 1
                print_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS,
                "\r   Generating random gradient grid... ", "");
            }
        }
    }

    if (display_loading != 0)
    {
        printf("\n");
    }
}



vector* getVector(gradientGrid* p_gradGrid, int width_idx, int height_idx)
{
    int width = p_gradGrid->width;
    int height = p_gradGrid->height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return (p_gradGrid->gradients) + height_idx * width + width_idx;
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
            vector* v = getVector(p_gradGrid, j, i); //(p_gradGrid->gradients) + i * width + j;

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
