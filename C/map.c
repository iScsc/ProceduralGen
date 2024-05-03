#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"

double* getMapValue(map* map, int width_idx, int height_idx)
{
    double* map_value = NULL;

    if (map != NULL)
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

        map_value = (map->map_values) + height_idx * width + width_idx;
    }

    return map_value;
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





map* newMapFromChunks(int map_width, int map_height, chunk* chunks[map_width * map_height], unsigned int display_loading)
{
    clock_t start_time = clock();

    if (chunks != NULL && map_width > 0 && map_height > 0)
    {
        map* new_map = calloc(1, sizeof(map));

        new_map->map_width = map_width;
        new_map->map_height = map_height;

        // copy chunks list to ensure dynamic allocation
        chunk** chunks_list = calloc(map_width * map_height, sizeof(chunk*));
        for (int i = 0; i < map_width * map_height; i++)
        {
            chunks_list[i] = chunks[i];
        }
        new_map->chunks = chunks_list;


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
                
                if (display_loading != 0)
                {
                    int nb_indents = display_loading - 1;

                    char base_str[100] = "Generating map values...           ";

                    predefined_loading_bar(j + i * width, width * height - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
                }
            }
        }

        return new_map;
    }
    else
    {
        return NULL;
    }
}



map* newMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                 int size_factors[number_of_layers], double layers_factors[number_of_layers], int map_width, int map_height, unsigned int display_loading)
{
    clock_t start_time = clock();

    chunk* chunks[map_width * map_height];

    int c_loading = display_loading;
    
    if (display_loading != 0)
    {
        indent_print(display_loading - 1, "Chunks generation before generating map...\n");
        c_loading += 2;
    }

    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            clock_t chunk_start_time = clock();

            chunk* current_chunk = NULL;

            if (display_loading != 0)
            {
                char to_print[200] = "";
                snprintf(to_print, sizeof(to_print), "Generating chunk %d/%d...\n", i * map_width + j + 1, map_width * map_height);

                indent_print(display_loading, to_print);
            }

            if (i == 0 && j == 0)
            {
                current_chunk = newChunk(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, c_loading);
            }
            else
            {
                chunk* north_chunk = NULL;
                chunk* west_chunk = NULL;

                if (j > 0)
                {
                    west_chunk = chunks[i * map_width + j - 1];
                }
                if (i > 0)
                {
                    north_chunk = chunks[(i - 1) * map_width + j];
                }

                current_chunk = newAdjacentChunk(north_chunk, west_chunk, c_loading);
            }

            chunks[i * map_width + j] = current_chunk;
            
            if (display_loading != 0)
            {
                double total_time = (double) (clock() - chunk_start_time)/CLOCKS_PER_SEC;
                char final_string[200] = "";

                snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The chunk generation took a total of %.4lf second(s).\n",
                                        GREEN_COLOR, DEFAULT_COLOR, total_time);
                
                int nb_indents = display_loading;
                indent_print(nb_indents, final_string);

                indent_print(display_loading, "\n");
            }
        }
    }

    map* new_map = newMapFromChunks(map_width, map_height, chunks, display_loading);

    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The map generation took a total of %.4lf second(s).\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_map;
}





void writeMapFile(map* map, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Map\n");

        // Writing the parameters
        int map_width = map->map_width;
        int map_height = map->map_height;
        int chunk_width = map->chunk_width;
        int chunk_height = map->chunk_height;

        fprintf(f, "map_width=%d\nmap_height=%d\nchunk_width=%d\nchunk_height=%d\n", map_width, map_height, chunk_width, chunk_height);

        int width = map_width * chunk_width;
        int height = map_height * chunk_height;

        // Writing the values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double value = *getMapValue(map, j, i);

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

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



map* readMapFile(char path[]);





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
                    freeChunk(getChunk(map, j, i));
                }
            }

            free(map->chunks);
        }

        free(map);
    }
}
