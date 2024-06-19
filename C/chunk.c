/**
 * @file chunk.c
 * @author Zyno and BlueNZ
 * @brief chunk structure and functions implementation
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"

double* getChunkValue(chunk* chunk, int width_idx, int height_idx)
{
    double* chunk_value = NULL;

    if (chunk != NULL)
    {
        int width = chunk->width;
        int height = chunk->height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading chunk value. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading chunk value. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }

        chunk_value = (chunk->chunk_values) + height_idx * width + width_idx;
    }

    return chunk_value;
}



chunk* initChunk(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers])
{
    chunk* new_chunk = calloc(1, sizeof(chunk));

    double* chunk_values = calloc(width * height, sizeof(double));

    new_chunk->number_of_layers = number_of_layers;


    // Copy layer factors to ensure dynamic allocation
    double* factors = calloc(number_of_layers, sizeof(double));
    for (int i = 0; i < number_of_layers; i++)
    {
        factors[i] = layers_factors[i];
    }
    new_chunk->layers_factors = factors;


    // Copy layers list to ensure dynamic allocation
    layer** layers_list = calloc(number_of_layers, sizeof(layer*));
    for (int i = 0; i < number_of_layers; i++)
    {
        layers_list[i] = layers[i];
    }
    new_chunk->layers = layers_list;


    new_chunk->width = width;
    new_chunk->height = height;
    new_chunk->chunk_values = chunk_values;

    new_chunk->base_altitude=0;

    return new_chunk;
}





void regenerateChunk(chunk* chunk, unsigned int display_loading)
{
    clock_t start_time = clock();

    int width = chunk->width;
    int height = chunk->height;
    int nblayers = chunk->number_of_layers;
    double* factors = chunk->layers_factors;
    layer** layers = chunk->layers;

    // Summing the layers altitude and normalizing it with the divisor variable.
    // It is a weighted average of the altitude by the layers_factors.
    double divisor = 0.;

    for (int k = 0; k < nblayers; k++)
    {
        divisor += factors[k];
    }

    if (divisor == 0)
    {
        printf("%sChunk layers_factors sum up to 0! Invalid division incoming - returning now.%s", RED_COLOR, DEFAULT_COLOR);
        return;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getChunkValue(chunk, j, i);

            for (int k = 0; k < nblayers; k++)
            {
                double layer_value = *(getLayerValue(layers[k], j, i));

                *value += factors[k] * layer_value;
                
                if (display_loading != 0)
                {
                    int nb_indents = display_loading - 1;

                    char base_str[100] = "Generating chunk values...         ";

                    predefined_loading_bar((i * width + j) * nblayers + k, width * height * nblayers - 1, NUMBER_OF_SEGMENTS,
                                                base_str, nb_indents, start_time);
                }
            }

            *value /= divisor;
        }
    }
    chunk->base_altitude=-0.5+2*rand()*1./RAND_MAX;
}





chunk* newChunkFromLayers(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers],
                             unsigned int display_loading)
{
    chunk* new_chunk = initChunk(width, height, number_of_layers, layers_factors, layers);

    regenerateChunk(new_chunk, display_loading);

    return new_chunk;
}



chunk* newChunkFromGradients(int width, int height, int number_of_layers, gradientGrid* gradient_grids[number_of_layers], 
                                int size_factors[number_of_layers], double layers_factors[number_of_layers], unsigned int display_loading)
{
    layer* layers[number_of_layers];

    int g_loading = display_loading;

    if (display_loading != 0)
    {
        // Indenting once more the layer generation
        g_loading += 1;
    }

    // Generating the required layers
    for (int i = 0; i < number_of_layers; i++)
    {
        if (display_loading != 0)
        {
            char to_print[200] = "";
            snprintf(to_print, sizeof(to_print), "Generating layer %d/%d from gradient grid...\n", i + 1, number_of_layers);

            indent_print(display_loading - 1, to_print);
        }

        // Size_factors should match gradient_grids dimensions - 1
        layers[i] = newLayerFromGradient(gradient_grids[i], size_factors[i], g_loading);


        if (display_loading != 0)
        {
            indent_print(display_loading - 1, "\n");
        }
    }

    // Generating the chunk
    return newChunkFromLayers(width, height, number_of_layers, layers_factors, layers, display_loading);
}



chunk* newChunk(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers], int size_factors[number_of_layers], 
                        double layers_factors[number_of_layers], unsigned int display_loading)
{
    clock_t start_time = clock();

    layer* layers[number_of_layers];

    int l_loading = display_loading;

    if (display_loading != 0)
    {
        // Indenting two times more the layer generation
        l_loading += 2;
        indent_print(display_loading - 1, "Layer generation before generating chunk...\n");
    }

    // Generating the layers from scratch with the given parameters
    for (int i = 0; i < number_of_layers; i++)
    {
        if (display_loading != 0)
        {
            char to_print[200] = "";
            snprintf(to_print, sizeof(to_print), "Generating layer %d/%d...\n", i + 1, number_of_layers);

            indent_print(display_loading, to_print);
        }

        layers[i] = newLayer(gradGrids_width[i], gradGrids_height[i], size_factors[i], l_loading);

        if (display_loading != 0)
        {
            indent_print(display_loading, "\n");
        }
    }

    // Size_factors should match gradient_grids dimensions - 1
    int width = (gradGrids_width[0] - 1) * size_factors[0];
    int height = (gradGrids_height[0] - 1) * size_factors[0];
    
    // Generating the chunk
    chunk* new_chunk = newChunkFromLayers(width, height, number_of_layers, layers_factors, layers, display_loading);

    // Printing the time elapsed
    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The chunk generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_chunk;
}

chunk* newVirtualChunk(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers], int size_factors[number_of_layers], double layers_factors[number_of_layers])
{
    chunk* new_chunk = calloc(1, sizeof(chunk));

    new_chunk->number_of_layers = number_of_layers;

    // size_factors should match gradient_grids dimensions - 1
    int width = (gradGrids_width[0] - 1) * size_factors[0];
    int height = (gradGrids_height[0] - 1) * size_factors[0];
    new_chunk->width = width;
    new_chunk->height = height;

    // copy layer factors to ensure dynamic allocation
    double* factors = calloc(number_of_layers, sizeof(double));
    for (int i = 0; i < number_of_layers; i++)
    {
        factors[i] = layers_factors[i];
    }
    new_chunk->layers_factors = factors;

    new_chunk->chunk_values = NULL;

    new_chunk->layers = NULL;

    new_chunk->base_altitude=-0.5+2*rand()*1./RAND_MAX;

    return new_chunk;
}




chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk, unsigned int display_loading)
{
    clock_t start_time = clock();

    int width = 0;
    int height = 0;
    int nb_layers = 0;
    double* factors = NULL;

    // Tests on adjacent chunks
    if (north_chunk != NULL && west_chunk != NULL)
    {
        int n_width = north_chunk->width;
        int n_height = north_chunk->height;
        int n_nblayers = north_chunk->number_of_layers;
        double* n_factors = north_chunk->layers_factors;

        int w_width = west_chunk->width;
        int w_height = west_chunk->height;
        int w_nblayers = west_chunk->number_of_layers;
        double* w_factors = west_chunk->layers_factors;

        if (n_width != w_width)
        {
            printf("%sInconsistent width for north and west chunks! (north = %d | west = %d)%s",
            RED_COLOR, n_width, w_width, DEFAULT_COLOR);
            return NULL;
        }
        if (n_height != w_height)
        {
            printf("%sInconsistent height for north and west chunks! (north = %d | west = %d)%s",
            RED_COLOR, n_height, w_height, DEFAULT_COLOR);
            return NULL;
        }
        if (n_nblayers != w_nblayers)
        {
            printf("%sInconsistent number of layers for north and west chunks! (north = %d | west = %d)%s",
            RED_COLOR, n_nblayers, w_nblayers, DEFAULT_COLOR);
            return NULL;
        }

        for (int i = 0; i < n_nblayers; i++)
        {
            if (n_factors[i] != w_factors[i])
            {
                printf("%sInconsistent factor at index i = %d for north and west chunks! (north = %lf | west = %lf)%s",
                RED_COLOR, i, n_factors[i], w_factors[i], DEFAULT_COLOR);
                return NULL;
            }
        }


        width = n_width;
        height = n_height;
        nb_layers = n_nblayers;

        // Copying the arrays is not necessary here. It will be done by the `newChunkFromGradients` function
        factors = n_factors;
    }
    else if (north_chunk != NULL)
    {
        width = north_chunk->width;
        height = north_chunk->height;
        nb_layers = north_chunk->number_of_layers;

        // Copying the arrays is not necessary here. It will be done by the `newChunkFromGradients` function
        factors = north_chunk->layers_factors;
    }
    else if (west_chunk != NULL)
    {
        width = west_chunk->width;
        height = west_chunk->height;
        nb_layers = west_chunk->number_of_layers;

        // Copying the arrays is not necessary here. It will be done by the `newChunkFromGradients` function
        factors = west_chunk->layers_factors;
    }
    else
    {
        printf("%sBoth north and west chunks are null!%s", RED_COLOR, DEFAULT_COLOR);
        return NULL;
    }


    int c_loading = display_loading;
    if (display_loading != 0)
    {
        // Indenting once more the adjacent gradient grid generation
        indent_print(display_loading - 1, "Generating adjacent chunk...\n");
        c_loading += 1;
    }

    // GradientGrid generation
    gradientGrid* gradientGrids[nb_layers];
    int size_factors[nb_layers];

    for (int k = 0; k < nb_layers; k++)
    {
        gradientGrid* north_grid = NULL;
        gradientGrid* west_grid = NULL;

        if (north_chunk != NULL)
        {
            north_grid = north_chunk->layers[k]->gradient_grid;
            size_factors[k] = north_chunk->layers[k]->size_factor;
        }
        if (west_chunk != NULL)
        {
            west_grid = west_chunk->layers[k]->gradient_grid;
            size_factors[k] = west_chunk->layers[k]->size_factor;
        }

        gradientGrids[k] = newAdjacentGradGrid(north_grid, west_grid, c_loading);

        if (display_loading != 0)
        {
            indent_print(display_loading, "\n");
        }
    }

    // Chunk generation
    chunk* new_chunk = newChunkFromGradients(width, height, nb_layers, gradientGrids, size_factors, factors, c_loading);

    // Printing the time elapsed
    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The adjacent chunk generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_chunk;
}





chunk* copyChunk(chunk* p_chunk)
{
    chunk* res = calloc(1,sizeof(chunk));

    res->width=p_chunk->width;
    res->height=p_chunk->height;
    int n = res->width;
    int m = res->height;

    res->number_of_layers=p_chunk->number_of_layers;
    int nbr = res->number_of_layers;

    res->layers_factors = calloc(nbr,sizeof(double));
    for (int i=0; i<nbr; i++)
    {
        res->layers_factors[i]=p_chunk->layers_factors[i];
    }

    res->layers = calloc(nbr,sizeof(layer*));
    for (int i=0; i<nbr; i++)
    {
        res->layers[i]=copyLayer(p_chunk->layers[i]);
    }

    res->chunk_values = calloc(n*m, sizeof(double));
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++) 
        {
            *getChunkValue(res,i,j)=*getChunkValue(p_chunk,i,j);
        }
    }

    res->base_altitude=p_chunk->base_altitude;

    return res;
}





void writeChunkFile(chunk* chunk, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Chunk\n");

        // Writing the parameters
        int number_of_layers = chunk->number_of_layers;
        int width = chunk->width;
        int height = chunk->height;
        double base_altitude = chunk->base_altitude;

        fprintf(f, "number_of_layers=%d\nwidth=%d\nheight=%d\nbase_altitude=%8lf\n", number_of_layers, width, height, base_altitude);

        // Writing the values
        if (chunk->chunk_values!=NULL)
        {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    double value = *getChunkValue(chunk, j, i);

                    if (j != width - 1)
                    {
                        fprintf(f, "% .8lf\t", value);
                    }
                    else
                    {
                        fprintf(f, "% .8lf\n", value);
                    }
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



chunk* readChunkFile(char path[]);





void printChunk(chunk* chunk)
{
    int width = chunk->width;
    int height = chunk->height;

    int nb_layer = chunk->number_of_layers;

    double* factors = chunk->layers_factors;

    printf("-------------------------------------------\n");
    printf("Printing chunk of size = (%d, %d)\n", height, width);

    printf("It has %d layers with factors : {", nb_layer);

    for (int k = 0; k < nb_layer; k++)
    {
        if (k < nb_layer - 1)
        {
            printf("%lf, ", factors[k]);
        }
        else
        {
            printf("%lf}\n", factors[k]);
        }
    }
    printf("\n");

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getChunkValue(chunk, j, i);

            printf("%lf   ", *value);
        }
        printf("\n");
    }

    printf("-------------------------------------------\n");
}



void freeChunk(chunk* chunk)
{
    if (chunk != NULL)
    {
        if (chunk->layers_factors != NULL)
        {
            free(chunk->layers_factors);
        }

        if (chunk->chunk_values != NULL)
        {
            free(chunk->chunk_values);
        }


        if (chunk->layers != NULL)
        {
            for (int i=0; i < chunk->number_of_layers; i++)
            {
                freeLayer(chunk->layers[i]);
            }

            free(chunk->layers);
        }

        free(chunk);
    }
}
