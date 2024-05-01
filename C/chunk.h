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

// TODO : If needed, may be implemented in the future.
// // --- Layer List ---

// layerList* newLayerList();

// void addLayer(layerList* list, layer* layer);

// void removeLayer(layerList* list, int index);


// --- Chunks ---

// TODO : If needed, may be implemented in the future. (linked list required in order to adapt list length)
// void addLayer(chunk* chunk, double layer_factor);

// void addLayers(chunk* chunk, double layers_factors[number_of_layers]);

double* getChunkValue(chunk* chunk, int width_idx, int height_idx);

chunk* initChunk(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers]);



void regenerateChunk(chunk* chunk, unsigned int display_loading);



chunk* newChunkFromLayers(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers],
                             unsigned int display_loading);

chunk* newChunkFromGradients(int width, int height, int number_of_layers, gradientGrid* gradient_grids[number_of_layers], 
                                int size_factors[number_of_layers], double layers_factors[number_of_layers], unsigned int display_loading);

chunk* newChunk(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers], int size_factors[number_of_layers], 
                        double layers_factors[number_of_layers], unsigned int display_loading);



chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk, unsigned int display_loading);



void printChunk(chunk* chunk);

void freeChunk(chunk* chunk);

#endif
