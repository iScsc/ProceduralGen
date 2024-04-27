#include <malloc.h>
#include <stdio.h>
#include <time.h>

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



chunk* initChunk(int width, int height, int number_of_layers, double* layers_factors, layer** layers)
{
    chunk* new_chunk = calloc(1, sizeof(chunk));

    double* chunk_values = calloc(width * height, sizeof(double));

    new_chunk->number_of_layers = number_of_layers;
    new_chunk->layers_factors = layers_factors;
    new_chunk->layers = layers;

    new_chunk->width = width;
    new_chunk->height = height;
    new_chunk->chunk_values = chunk_values;

    return new_chunk;
}





void regenerateChunk(chunk* chunk, int display_loading)
{
    clock_t start_time = clock();

    int width = chunk->width;
    int height = chunk->height;
    int nblayers = chunk->number_of_layers;
    double* factors = chunk->layers_factors;
    layer** layers = chunk->layers;

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
                    double current_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;

                    char time_str[100];
                    sprintf(time_str, " - Elapsed time : %.3lf s", current_time);

                    // max :  (width * (height - 1) + (width - 1)) * nblayers + nblayers - 1  = (width * height - 1) * nblayers + nb_layers - 1
                    //                                                                        = width * height * nblayers - 1
                    print_loading_bar((i * width + j) * nblayers + k, width * height * nblayers - 1, NUMBER_OF_SEGMENTS,
                    "\r   Generating chunk...                ", time_str);
                }
            }

            *value /= divisor;
        }
    }

    if (display_loading != 0)
    {
        printf("\n");
    }
}





chunk* newChunkFromLayers(int width, int height, int number_of_layers, double* layers_factors, layer** layers, int display_loading)
{
    chunk* new_chunk = initChunk(width, height, number_of_layers, layers_factors, layers);

    regenerateChunk(new_chunk, display_loading);

    return new_chunk;
}



chunk* newChunkFromGradients(int width, int height, int number_of_layers, gradientGrid** gradient_grids, int* size_factors,
                             double* layers_factors, int display_loading)
{
    layer** layers = calloc(number_of_layers, sizeof(layer*));

    for (int i = 0; i < number_of_layers; i++)
    {
        // size_factors should match gradient_grids dimensions - 1
        layers[i] = newLayerFromGradient(gradient_grids[i], size_factors[i], display_loading);
    }

    return newChunkFromLayers(width, height, number_of_layers, layers_factors, layers, display_loading);
}



chunk* newChunk(int number_of_layers, int* gradGrids_width, int* gradGrids_height, int* size_factors, double* layers_factors, int display_loading)
{
    layer** layers = calloc(number_of_layers, sizeof(layer*));

    printf("Layer generation before generating chunk...\n");
    for (int i = 0; i < number_of_layers; i++)
    {
        layers[i] = newLayer(gradGrids_width[i], gradGrids_height[i], size_factors[i], display_loading);
    }

    // size_factors should match gradient_grids dimensions - 1
    int width = (gradGrids_width[0] - 1) * size_factors[0];
    int height = (gradGrids_height[0] - 1) * size_factors[0];
    
    return newChunkFromLayers(width, height, number_of_layers, layers_factors, layers, display_loading);
}





chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk, int display_loading)
{
    int width = 0;
    int height = 0;
    int nb_layers = 0;
    double* factors = NULL;

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
        factors = n_factors;
    }
    else if (north_chunk != NULL)
    {
        width = north_chunk->width;
        height = north_chunk->height;
        nb_layers = north_chunk->number_of_layers;
        factors = north_chunk->layers_factors;
    }
    else if (west_chunk != NULL)
    {
        width = west_chunk->width;
        height = west_chunk->height;
        nb_layers = west_chunk->number_of_layers;
        factors = west_chunk->layers_factors;
    }
    else
    {
        printf("%sBoth north and west chunks are null!%s", RED_COLOR, DEFAULT_COLOR);
        return NULL;
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

        gradientGrids[k] = newAdjacentGradGrid(north_grid, west_grid, display_loading);
    }

    chunk* new_chunk = newChunkFromGradients(width, height, nb_layers, gradientGrids, size_factors, factors, display_loading);

    return new_chunk;
}





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
            printf("Freeing layers factors\n");
            free(chunk->layers_factors);
            printf("Layers factors freed\n");
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
