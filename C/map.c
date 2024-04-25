#include <stdio.h>
#include <malloc.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"

double* getMapValue(map* map, int width_idx, int height_idx)
{
    int width = map->map_width * map->chunk_width;
    int height = map->map_height * map->chunk_height;

    if (width_idx < 0 || width_idx >= width)
    {
        printf("%sERROR : invalid width_idx = %d when reading map value. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        printf("%sERROR : invalid height_idx = %d when reading map value. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
        return NULL;
    }

    return (map->map_values) + height_idx * width + width_idx;
}



chunk* getChunk(map* map, int width_idx, int height_idx)
{
    int width = map->map_width;
    int height = map->map_height;

    if (width_idx < 0 || width_idx >= width)
    {
        return NULL;
    }
    
    if (height_idx < 0 || height_idx >= height)
    {
        return NULL;
    }

    return map->chunks[height_idx * width + width_idx];
}





map* newMapFromChunks(int map_width, int map_height, chunk** chunks)
{

    if (chunks != NULL && map_width > 0 && map_height > 0)
    {
        map* new_map = calloc(1, sizeof(map));

        new_map->map_width = map_width;
        new_map->map_height = map_height;

        new_map->chunks = chunks;



        chunk* firstChunk = getChunk(new_map, 0, 0);
        int chunk_width = firstChunk->width;
        int chunk_height = firstChunk->height;

        new_map->chunk_width = chunk_width;
        new_map->chunk_height = chunk_height;


        int width = map_width * chunk_width;
        int height = map_height * chunk_height;
        double* map_values = calloc(width * height, sizeof(double));

        new_map->map_values = map_values;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double* value = getMapValue(new_map, j, i);
                chunk* current_chunk = getChunk(new_map, j/chunk_width, i/chunk_height);

                *value = *getChunkValue(current_chunk, j%chunk_width, i%chunk_height);
            }
        }

        return new_map;
    }
    else
    {
        return NULL;
    }
}



map* newMap(int number_of_layers, int* gradGrids_width, int* gradGrids_height, int* size_factors, double* layers_factors, int map_width, int map_height)
{
    chunk** chunks = calloc(map_width * map_height, sizeof(chunk*));

    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            chunk* current_chunk = newChunk(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors);

            chunks[i * map_width + j] = current_chunk;
        }
    }

    return newMapFromChunks(map_width, map_height, chunks);
}





void printMap(map* map)
{
    int map_width = map->map_width;
    int map_height = map->map_height;

    int chunk_width = map->chunk_width;
    int chunk_height = map->chunk_height;

    printf("-------------------------------------------\n");
    printf("Printing map of size (%d, %d), with each chunk of size (%d, %d)\n\n", map_height, map_width, chunk_height, chunk_width);

    for (int i = 0; i < map_height * chunk_height; i++)
    {
        for (int j = 0; j < map_width * chunk_width; j++)
        {
            double* value = getMapValue(map, j, i);

            if (j != 0 && j%chunk_width == 0)
            {
                printf(" | ");
            }

            printf("% .4lf   ", *value);
        }
        printf("\n");
        if (i != 0 && i%chunk_height == 0)
        {
            for (int j = 0; j < map_width * chunk_width; j++)
            {
                if (j != 0 && j%chunk_width == 0)
                {
                    printf("   ");
                }

                printf("----------");
            }
            printf("\n");
        }
    }

    printf("-------------------------------------------\n");
}



void freeMap(map* map)
{
    if (map != NULL)
    {
        int map_width = map->map_width;
        int map_height = map->map_height;

        if (map->map_values != NULL)
        {
            free(map->map_values);
        }


        if (map->chunks != NULL)
        {
            for (int i = 0; i < map_height; i++)
            {
                for (int j = 0; j < map_width; j++)
                {
                    // Be aware that layers_factors may be the same list referenced in each chunk if chunks were created here. Should not be freed several times.
                    // TODO: Should rather be cloned ! Verify every other lists. Should be copied in structures, not directly referenced.
                    // TODO: This way, users will be able to create local arrays and pass them as well. Or they can create arrays dynamically and free them themselves.
                    
                    freeChunk(getChunk(map, j, i));
                }
            }

            free(map->chunks);
        }

        free(map);
    }
}
