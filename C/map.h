#ifndef MAP
#define MAP

#include "chunk.h"

// ----- Structure definition -----

struct map
{
    int map_width; // number of chunks in width
    int map_height; // number of chunks in height
    chunk** chunks;

    int chunk_width;
    int chunk_height;
    double* map_values; // size : map_width * chunk_width x map_height * chunk_height
};

typedef struct map map;

// ----- Functions -----

double* getMapValue(map* map, int width_idx, int height_idx);

chunk* getChunk(map* map, int width_idx, int height_idx);



map* newMapFromChunks(int map_width, int map_height, chunk** chunks, int display_loading);

map* newMap(int number_of_layers, int* gradGrids_width, int* gradGrids_height, int* size_factors, double* layers_factors,
            int map_width, int map_height, int display_loading);



void printMap(map* map);

void freeMap(map* map);

#endif
