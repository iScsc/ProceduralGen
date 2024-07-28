/**
 * @file gradientGrid.c
 * @author Zyno and BlueNZ
 * @brief gradientGrid structure implementation
 * @version 0.3
 * @date 2024-07-28
 * 
 */

#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "loadingBar.h"
#include "interpreter.h"
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





void regenerateRandomGradGrid(gradientGrid* gradGrid, unsigned int display_loading)
{
    clock_t start_time = clock();

    int width = gradGrid->width;
    int height = gradGrid->height;

    // Generating random vectors
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            vector* v = getVector(gradGrid, j, i);

            // Generating a random sign for x value
            int sign_x = 1 - 2 * (rand()%2);
            v->x = sign_x * (double) rand()/RAND_MAX;

            // Generating a random sign for y value
            int sign_y = 1 - 2 * (rand()%2);
            v->y = sign_y * sqrt(1. - v->x * v->x);

            if (display_loading != 0)
            {
                char base_str[100] = "Generating random gradient grid... ";

                int nb_indents = (display_loading - 1);

                // Max index :  (width - 1) + (height - 1) * width  =  width * height - 1
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

    //! Vector array calloc'd but not initialized
    new_grad_grid->gradients = gradients;

    return new_grad_grid;
}



gradientGrid* newRandomGradGrid(int width, int height, unsigned int display_loading)
{
    // Generating a new gradientGrid with not initialized vectors
    gradientGrid* new_grad_grid = newGradGrid(width, height);

    // Initializing vectors to random ones
    regenerateRandomGradGrid(new_grad_grid, display_loading);

    return new_grad_grid;
}





gradientGrid* newAdjacentGradGrid(gradientGrid* north_grid, gradientGrid* west_grid, unsigned int display_loading)
{
    clock_t start_time = clock();

    int width = 0;
    int height = 0;

    // Tests on given north and west grids
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


    // Begin of the random generation
    if (display_loading != 0)
    {
        char begin_string[200] = "";
        snprintf(begin_string, sizeof(begin_string), "Generating adjacent gradient grid of size %d x %d...\n", width, height);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, begin_string);
    }



    int display_grad_grid = display_loading;
    if (display_loading != 0)
    {
        // Indenting once more the random generation
        display_grad_grid += 1;
    }

    // First generates a random grid
    gradientGrid* new_grad_grid = newRandomGradGrid(width, height, display_grad_grid);

    // Then applies boundary conditions for the newly generated grid
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

    // Printing the time elapsed.
    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The adjacent gradient grid generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_grad_grid;
}





gradientGrid* copyGrad(gradientGrid* grad) 
{
    gradientGrid* res = calloc(1,sizeof(gradientGrid));

    res->width=grad->width;
    res->height=grad->height;

    res->gradients = calloc(res->width*res->height,sizeof(vector));
    for (int i=0; i<res->width; i++)
    {
        for (int j=0; j<res->height; j++)
        {
            vector* vect=copyVect(getVector(grad,i,j));
            *getVector(res,i,j)=*vect;
            free(vect);
        }
    }

    return res;

}



vector* copyVect(vector* vect) 
{
    vector*res = calloc(1, sizeof(vector));

    res->x=vect->x;
    res->y=vect->y;

    return res;
}





bytes bytesGradientGrid(gradientGrid* grid) {
    bytes bytes_str;
    bytes_str.bytes = malloc(2*(grid->height*grid->width+1)*INT_BITS_NBR/8+1);
    bytes_str.size = 2*(grid->height*grid->width+1)*INT_BITS_NBR/8+1;
    bytes_str.start = 0;

    bytes_str.bytes[0] = GRID_ENCODING;

    bytes a = bytesInt(grid->height);
    concatBytes(bytes_str, a, 1);
    freeBytes(a);
    a = bytesInt(grid->width);
    concatBytes(bytes_str, a, 1+INT_BITS_NBR/8);
    freeBytes(a);

    for (int i=0; i<grid->height; i++) {
        for (int j=0; j<grid->width; j++) {
            vector vect = *getVector(grid,j,i);
            a = bytesDouble(vect.x);
            concatBytes(bytes_str, a, 1 + 2 * INT_BITS_NBR/8 + 2 * (i * grid->width + j) * FLOAT_BITS_NBR/8);
            freeBytes(a);
            a = bytesDouble(vect.y);
            concatBytes(bytes_str, a, 1 + 2 * INT_BITS_NBR/8 + (2 * (i * grid->width + j) + 1) * FLOAT_BITS_NBR/8);
            freeBytes(a);
        }
    }

    return bytes_str;

}

tuple_obj_bytes nextGradientGrid(bytes bytes) {
    tuple_obj_bytes res;

    if (bytes.bytes[0]==GRID_ENCODING) {
        bytes.start += 1;

        tuple_obj_bytes a = nextInt(bytes);
        int h = *((int*)a.object);
        bytes = a.bytes;
        free(a.object);
        a = nextInt(bytes);
        int w = *((int*)a.object);
        bytes = a.bytes;
        free(a.object);

        gradientGrid* obj = newGradGrid(w,h);

        for (int i=0; i<h; i++) {
            for (int j=0; j<w; j++) {
                vector* vect = getVector(obj,j,i);
                a = nextDouble(bytes);
                vect->x = *((double*)a.object);
                bytes = a.bytes;
                free(a.object);
                a = nextDouble(bytes);
                vect->y = *((double*)a.object);
                bytes = a.bytes;
                free(a.object);
            }
        }

        res.object = (object) obj;
        res.bytes = bytes;
    }

    return res;
}

void writeGradientGridFile(gradientGrid* gradGrid, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Gradient Grid\n");

        // Writing the parameters
        int width = gradGrid->width;
        int height = gradGrid->height;

        fprintf(f, "width=%d\nheight=%d\n", width, height);

        // Writing the values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                vector* vec = getVector(gradGrid, j, i);

                if (j != width - 1)
                {
                    fprintf(f, "(% .8lf,% .8lf)\t", vec->x, vec->y);
                }
                else
                {
                    fprintf(f, "(% .8lf,% .8lf)\n", vec->x, vec->y);
                }
            }
        }

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



// TODO : implement reading function if required.
gradientGrid* readGradientGridFile(char path[]);





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
        // Every gradients will be free'd.
        if (gradGrid->gradients != NULL)
        {
            free(gradGrid->gradients);
        }

        free(gradGrid);
    }
}
