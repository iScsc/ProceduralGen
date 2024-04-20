#include <malloc.h>

#include "gradientGrid.h"

gradientGrid* newGradGrid(int width, int height)
{
    gradientGrid* new_grad_grid = calloc(1, sizeof(gradientGrid));

    vector* gradients = calloc(width * height, sizeof(vector));

    new_grad_grid->width = width;
    new_grad_grid->height = height;
    new_grad_grid->gradients = gradients;

    return new_grad_grid;
}

void freeGradGrid(gradientGrid* p_gradGrid)
{
    free(p_gradGrid->gradients);
    free(p_gradGrid);
}
