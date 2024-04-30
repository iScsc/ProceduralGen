#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "loadingBar.h"
#include "gradientGrid.h"

void setRandomSeed(unsigned int seed)
{
    srand(seed);
}



vector* getVector(gradientGrid* gradGrid, int width_idx, int height_idx)
{
    vector* vec = NULL;

    if (gradGrid != NULL)
    {
        int width = gradGrid->width;
        int height = gradGrid->height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading gradient grid vector. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading gradient grid vector. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }

        vec = (gradGrid->gradients) + height_idx * width + width_idx;
    }

    return vec;
}





void regenerateRandomGradGrid(gradientGrid* gradGrid, int display_loading)
{
    clock_t start_time = clock();

    int width = gradGrid->width;
    int height = gradGrid->height;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = getVector(gradGrid, j, i);

            v->x = (double) rand()/RAND_MAX;
            v->y = sqrt(1. - v->x * v->x);

            if (display_loading != 0)
            {
                char base_str[100] = "Generating random gradient grid... ";

                int nb_indents = (display_loading - 1);

                // max :  (width - 1) + (height - 1) * width  =  width * height - 1
                predefined_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
            }
        }
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
    clock_t start_time = clock();

    int width = 0;
    int height = 0;

    // Tests on pointers
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


    // Begin of the generation
    if (display_loading != 0)
    {
        char begin_string[200] = "Generating an adjacent gradient grid...\n";
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, begin_string);
    }



    int display_grad_grid = display_loading;
    if (display_loading > 0)
    {
        display_grad_grid += 1;
    }

    gradientGrid* new_grad_grid = newRandomGradGrid(width, height, display_grad_grid);

    // Applying boundary conditions for the newly generated grid

    if (north_grid != NULL)
    {
        clock_t north_start_time = clock();
        for (int j = 0; j < width; j++)
        {
            vector* vec = getVector(new_grad_grid, j, 0);
            vector* north_vec = getVector(north_grid, j, height - 1);

            vec->x = north_vec->x;
            vec->y = north_vec->y;

            if (display_loading != 0)
            {
                char base_str[100] = "Applying North boundary conditions ";

                // +1 to indent once more than random adjacent gradient grid base text.
                int nb_indents = (display_loading - 1) + 1;

                predefined_loading_bar(j, width - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, north_start_time);
            }
        }
    }

    if (west_grid != NULL)
    {
        clock_t west_start_time = clock();
        for (int i = 0; i < height; i++)
        {
            vector* vec = getVector(new_grad_grid, 0, i);
            vector* west_vec = getVector(west_grid, width - 1, i);

            vec->x = west_vec->x;
            vec->y = west_vec->y;

            if (display_loading != 0)
            {
                char base_str[100] = "Applying West boundary conditions  ";

                // +1 to indent once more than random adjacent gradient grid base text.
                int nb_indents = (display_loading - 1) + 1;

                predefined_loading_bar(i, height - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, west_start_time);
            }
        }
    }

    if (display_loading != 0)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        sprintf(final_string, "%sSUCCESS :%s The adjacent gradient grid generation took a total of %.4lf second(s).\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_grad_grid;
}





void printGradientGrid(gradientGrid* gradGrid)
{
    int width = gradGrid->width;
    int height = gradGrid->height;

    printf("-------------------------------------------\n");
    printf("Printing gradient grid of size = (%d, %d)\n\n", height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = getVector(gradGrid, j, i);

            printf("(%lf, %lf)   ", v->x, v->y);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeGradGrid(gradientGrid* gradGrid)
{
    if (gradGrid != NULL)
    {
        if (gradGrid->gradients != NULL)
        {
            free(gradGrid->gradients);
        }

        free(gradGrid);
    }
}
