#ifndef CHUNCK
#define CHUNCK

#include "layer.h"

// ----- Structure definition -----

struct chunk
{
    int width;
    int height;

    int number_of_layers;
    double* layers_factors;
    layer** layers;

    double* chunk_values;
};

typedef struct chunk chunk;

// ----- Functions -----

void freeChunk(chunk* chunk);

#endif
