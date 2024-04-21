#ifndef MAP
#define MAP

#include "chunk.h"

// ----- Structure definition -----

struct map
{
    int map_width;
    int map_height;

    int chunk_width;
    int chunk_height;

    chunk** chunks;

    double* final_values;
};

typedef struct map map;

// ----- Functions -----


void freeMap(map* map);

#endif
