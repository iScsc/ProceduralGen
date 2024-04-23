#include <malloc.h>
#include <stdio.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"

double* getChunkValue(chunk* chunk, int width_idx, int height_idx)
{
    int width = chunk->width;
    int height = chunk->height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return (chunk->chunk_values) + height_idx * width + width_idx;
}



chunk* initChunk(int width, int height, int number_of_layers, double* layers_factors, layer** layers)
{
    chunk* new_chunk = calloc(1, sizeof(chunk));

    double* chunk_values = calloc(width * height, sizeof(double));

    new_chunk->width = width;
    new_chunk->height = height;

    new_chunk->number_of_layers = number_of_layers;
    new_chunk->layers_factors = layers_factors;
    new_chunk->layers = layers;

    new_chunk->chunk_values = chunk_values;

    return new_chunk;
}





void regenerateChunk(chunk* chunk)
{
    int width = chunk->width;
    int height = chunk->height;
    int nblayers = chunk->number_of_layers;
    double* factors = chunk->layers_factors;
    layer** layers = chunk->layers;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double* value = getChunkValue(chunk, j, i);

            for (int k = 0; k < nblayers; k++)
            {
                double layer_value = *(getLayerValue(layers[k], j, i));

                *value += factors[k] * layer_value;
            }
        }
    }
}





chunk* newChunkFromLayers(int width, int height, int number_of_layers, double* layers_factors, layer** layers)
{
    chunk* new_chunk = initChunk(width, height, number_of_layers, layers_factors, layers);

    regenerateChunk(new_chunk);

    return new_chunk;
}



chunk* newChunkFromGradient(int width, int height, int number_of_layers, gradientGrid** gradient_grids, int* size_factors, double* layers_factors)
{
    layer** layers = calloc(number_of_layers, sizeof(__SIZEOF_POINTER__));

    for (int i = 0; i < number_of_layers; i++)
    {
        layers[i] = newLayerFromGradient(gradient_grids[i], size_factors[i], 1);
    }

    return newChunkFromLayers(width, height, number_of_layers, layers_factors, layers);
}



chunk* newChunk(int width, int height, int number_of_layers, int* size_factors, double* layers_factors)
{
    layer** layers = calloc(number_of_layers, sizeof(__SIZEOF_POINTER__));

    for (int i = 0; i < number_of_layers; i++)
    {
        layers[i] = newLayer(width, height, size_factors[i], 1);
    }

    return newChunkFromLayers(width, height, number_of_layers, layers_factors, layers);
}





chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk)
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
                printf("%sInconsistent factor at index i = %d for north and west chunks! (north = %d | west = %d)%s",
                RED_COLOR, i, n_nblayers, w_factors, DEFAULT_COLOR);
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

    gradientGrid** gradientGrids = calloc(nb_layers, sizeof(__SIZEOF_POINTER__));
    int size_factors[nb_layers];

    for (int k = 0; k < nb_layers; k++)
    {
        gradientGrid* north_grid = NULL;
        gradientGrid* west_grid = NULL;west_chunk->layers[k]->gradient_grid;

        if (north_chunk != NULL)
        {
            north_grid = north_chunk->layers[k]->gradient_grid;
            size_factors[k] = north_chunk->layers[k]->size_factor;
        }
        if (west_chunk != NULL)
        {
            west_grid = west_chunk->layers[k]->gradient_grid;
            size_factors[k] = north_chunk->layers[k]->size_factor;
        }

        gradientGrids[k] = newAdjacentGradGrid(north_grid, west_grid, 1);
    }

    chunk* new_chunk = newChunkFromGradient(width, height, nb_layers, gradientGrids, size_factors, factors);

    return new_chunk;
}





void freeChunk(chunk* chunk)
{
    free(chunk->layers_factors);
    free(chunk->chunk_values);

    for (int i=0; i < chunk->number_of_layers; i++)
    {
        freeLayer(chunk->layers[i]);
    }

    free(chunk);
}
