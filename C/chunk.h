#ifndef CHUNCK
#define CHUNCK

#include "layer.h"

// ----- Structure definition -----

// TODO : In the future, if needed, may be implemented.
// struct linkedLayerNode
// {
//     layer* layer;

//     linkedLayerNode* next_node;
// };

// typedef linkedLayerNode layerNode;



// struct linkedLayerList
// {
//     int length;

//     layerNode head;
// };

// typedef linkedLayerList layerList;



struct chunk
{
    int number_of_layers;
    double* layers_factors;
    layer** layers;

    int width;
    int height;
    double* chunk_values;
};

typedef struct chunk chunk;

// ----- Functions -----

// TODO : In the future, if needed, may be implemented.
// // --- Layer List ---

// layerList* newLayerList();

// void addLayer(layerList* list, layer* layer);

// void removeLayer(layerList* list, int index);


// --- Chunks ---

// TODO : In the future, if needed, may be implemented. (need for linked list in order to adapt list length)
// void addLayer(chunk* chunk, double layer_factor);

// void addLayers(chunk* chunk, double* layers_factors);

double* getChunkValue(chunk* chunk, int width_idx, int height_idx);

chunk* initChunk(int width, int height, int number_of_layers, double* layers_factors, layer** layers);



void regenerateChunk(chunk* chunk, int display_loading);



chunk* newChunkFromLayers(int width, int height, int number_of_layers, double* layers_factors, layer** layers, int display_loading);

chunk* newChunkFromGradients(int width, int height, int number_of_layers, gradientGrid** gradient_grids, int* size_factors,
                             double* layers_factors, int display_loading);

chunk* newChunk(int number_of_layers, int* gradGrids_width, int* gradGrids_height, int* size_factors, double* layers_factors, int display_loading);



chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk, int display_loading);



void printChunk(chunk* chunk);

void freeChunk(chunk* chunk);

#endif
