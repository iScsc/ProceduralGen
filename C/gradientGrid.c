#include <malloc.h>
#include <stdlib.h>
#include <math.h>

#include "loadingBar.h"
#include "gradientGrid.h"

void setRandomSeed(unsigned int seed)
{
    srand(seed);
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





gradientGrid* newAdjacentGradGrid(gradientGrid* north_grid, gradientGrid* west_grid, int display_loading)
{
    int width = 0;
    int height = 0;

    if (north_grid != NULL && west_grid != NULL)
    {
        int n_width = north_grid->width;
        int n_height = north_grid->height;

        int w_width = west_grid->width;
        int w_height = west_grid->height;

        if (n_width != w_width)
        {
            printf("%sInconsistent width for north and west gradient grids! (north = %d | west = %d)%s",
            RED_COLOR, n_width, w_width, DEFAULT_COLOR);
            return NULL;
        }
        if (n_height != w_height)
        {
            printf("%sInconsistent height for north and west gradient grids! (north = %d | west = %d)%s",
            RED_COLOR, n_height, w_height, DEFAULT_COLOR);
            return NULL;
        }

        width = n_width;
        height = n_height;
    }
    else if (north_grid != NULL)
    {
        width = north_grid->width;
        height = north_grid->height;
    }
    else if (west_grid != NULL)
    {
        width = west_grid->width;
        height = west_grid->height;
    }
    else
    {
        printf("%sBoth north and west grids are null!%s", RED_COLOR, DEFAULT_COLOR);
        return NULL;
    }

    gradientGrid* new_grad_grid = newRandomGradGrid(width, height, display_loading);

    // Applying boundary conditions for the newly generated grid

    if (north_grid != NULL)
    {
        for (int j = 0; j < width; j++)
        {
            vector* vec = getVector(new_grad_grid, j, 0);
            vector* north_vec = getVector(north_grid, j, height - 1);

            vec->x = north_vec->x;
            vec->y = north_vec->y;

            if (display_loading != 0)
            {
                // max :  (width - 1) + (height - 1) * width  =  width * height - 1
                print_loading_bar(j, width - 1, NUMBER_OF_SEGMENTS,
                "\r   Applying North boundary conditions ", "");
            }
        }

        if (display_loading != 0)
        {
            printf("\n");
        }
    }

    if (west_grid != NULL)
    {
        for (int i = 0; i < height; i++)
        {
            vector* vec = getVector(new_grad_grid, 0, i);
            vector* west_vec = getVector(west_grid, width - 1, i);

            vec->x = west_vec->x;
            vec->y = west_vec->y;

            if (display_loading != 0)
            {
                // max :  (width - 1) + (height - 1) * width  =  width * height - 1
                print_loading_bar(i, height - 1, NUMBER_OF_SEGMENTS,
                "\r   Applying West boundary conditions  ", "");
            }
        }

        if (display_loading != 0)
        {
            printf("\n");
        }
    }

    return new_grad_grid;
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
