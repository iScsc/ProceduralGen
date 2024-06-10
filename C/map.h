#ifndef MAP
#define MAP

#include "chunk.h"

// ----- Structure definition -----

struct map
{
    int map_width; // number of chunks in width
    int map_height; // number of chunks in height
    chunk** chunks;

    chunk** virtual_chunks; // virtual chunks needed for border condition and futur map expanding (C0,CN,L0,LN)

    int chunk_width;
    int chunk_height;
    double* map_values; // size : map_width * chunk_width x map_height * chunk_height
};

typedef struct map map;

// ----- Functions -----

double* getMapValue(map* map, int width_idx, int height_idx);

chunk* getChunk(map* map, int width_idx, int height_idx);

chunk* getVirtualChunk(map* map, int width_idx, int height_idx);


double interpolate2D(double a1, double a2, double a3, double a4, double x, double y);

map* addMeanAltitude(map* p_map, unsigned int display_loading);


map* newMapFromChunks(int map_width, int map_height, chunk* chunks[map_width * map_height], chunk* virtual_chunks[(map_height+2+map_width+2)*2-4], unsigned int display_loading);

map* newMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                 int size_factors[number_of_layers], double layers_factors[number_of_layers], int map_width, int map_height, unsigned int display_loading);



void writeMapFile(map* map, char path[]);

map* readMapFile(char path[]);



void printMap(map* map);

void freeMap(map* map);

map* copyMap(map* p_map) ;

#endif
