/**
 * @file chunk.h
 * @author Zyno and BlueNZ
 * @brief Header to chunk structure and functions
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#ifndef CHUNK
#define CHUNK

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



/**
 * @brief A chunk structure.
 * 
 */
struct chunk
{
    int number_of_layers; /**< the number of layers in the chunk*/
    double* layers_factors; /**< the array of layers factors values*/
    layer** layers; /**< the array of pointers to the layer structures composing the chunk*/

    int width; /**< the width of the chunk (redundant with layers' width)*/
    int height; /**< the height of the chunk (redundant with layers' height)*/
    double* chunk_values; /**< the final chunk altitude values*/
    double base_altitude; /**< the base altitude of the chunk to generate inhomogeneous maps*/
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

/**
 * @brief Get the altitude value pointer from the given chunk at given indexes.
 * 
 * @param chunk (chunk*) : the pointer to the chunk to extract the value pointer from.
 * @param width_idx (int) : the width index of the wanted value.
 * @param height_idx (int) : the height index of the wanted value.
 * @return double* : the pointer to the corresponding altitude value.
 */
double* getChunkValue(chunk* chunk, int width_idx, int height_idx);

/**
 * @brief Initializes a chunk with the given parameters and allocates the space for the `values` array.
 * 
 * @param width (int) : the chunk width.
 * @param height (int) : the chunk height.
 * @param number_of_layers (int) : the number of layers in the chunk.
 * @param layers_factors (double[]) : the array containing the layers factors. Values will be copied in the structure as a double*.
 * @param layers (layer*[]) : the array of pointers to the layer structures composing the chunk. Pointers will be copied in the structure as a layer**.
 * @return chunk* : the pointer to the newly created and initialized chunk structure.
 * 
 * @note `layers_factors` and `layers` arrays does not require to be dynamically allocated.
 */
chunk* initChunk(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers]);



/**
 * @brief Regenerates the given chunk final altitude values from its layers and layers factors.
 * 
 * @param chunk (chunk*) : the pointer to the initialized chunk structure to regenerate the altitude values.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 */
void regenerateChunk(chunk* chunk, unsigned int display_loading);



/**
 * @brief Generates a new chunk structure from the given layers and parameters.
 * 
 * @param width (int) : the width of the passed layers and thus of the chunk structure.
 * @param height (int) : the height of the passed layers and thus of the chunk structure.
 * @param number_of_layers (int) : the number of layers passed.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param layers (layer*[number_of_layers]) : the array of pointers to the layers to generate the chunk from.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return chunk* : the pointer to the newly generated chunk structure.
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
chunk* newChunkFromLayers(int width, int height, int number_of_layers, double layers_factors[number_of_layers], layer* layers[number_of_layers],
                             unsigned int display_loading);

/**
 * @brief Generates a new chunk structure from the given gradientGrids and parameters.
 * 
 * @param width (int) : the width of the chunk structure.
 * @param height (int) : the height of the chunk structure.
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradient_grids (gradientGrid*[number_of_layers]) : the array of gradientGrid pointers to generate the layers and then the chunk from.
 * @param size_factors (int[number_of_layers]) : the array of size factors to generate the layers.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return chunk* : the pointer to the newly generated chunk structure.
 * 
 * @warning The `size_factors` array should match the gradientGrid dimensions and the passed width and height dimensions :
 *          `chunk_dimensions = (gradGrid_dimensions - 1) * size_factor`
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
chunk* newChunkFromGradients(int width, int height, int number_of_layers, gradientGrid* gradient_grids[number_of_layers], 
                                int size_factors[number_of_layers], double layers_factors[number_of_layers], unsigned int display_loading);

/**
 * @brief Generates a new chunk structure from scratch with the given parameters.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_width (int[number_of_layers]) : the array of gradientGrid width to be used to generate the random gradient grids.
 * @param gradGrids_height (int[number_of_layers]) : the array of gradientGrid height to be used to generate the random gradient grids.
 * @param size_factors (int[number_of_layers]) : the array of size factors to generate the layers.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return chunk* : the pointer to the newly generated chunk structure.
 * 
 * @warning The `size_factors` array should match the `gradGrids_width` and `gradGrids_height` arrays such that
 *          `(gradGrid_dimensions - 1) * size_factor = constant`. In case it would not be the case, the chunk structure will still 
 *          be generated but its dimensions will be wrong : it will simply use the first value. 
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
chunk* newChunk(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers], int size_factors[number_of_layers], 
                        double layers_factors[number_of_layers], unsigned int display_loading);

//TODO ? signature could be changed to avoid passing useless parameters -> `chunk_width` and `chunk_height` instead of `gradGrids_width`, `gradGrids_height` and `size_factors`
/**
 * @brief Generates a new virtual chunk structure with the given parameters. It does not possess any altitude values and is used as a way to store
 * the data of boundary conditions with its `base_altitude` parameter.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_width (int[number_of_layers]) : the array of gradientGrid width to compute the final width this virtual chunk should have.
 * @param gradGrids_height (int[number_of_layers]) : the array of gradientGrid height to compute the final height this virtual chunk should have.
 * @param size_factors (int[number_of_layers]) : the array of size factors to compute the final dimensions of this virtual chunk.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors to be stored in the virtual chunk.
 * @return chunk* : the pointer to the newly generated virtual chunk structure.
 * 
 * @warning The `size_factors` array should match the `gradGrids_width` and `gradGrids_height` arrays such that
 *          `(gradGrid_dimensions - 1) * size_factor = constant`. In case it would not be the case, the chunk structure will still 
 *          be generated but its dimensions will be wrong : it will simply use the first value. 
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
chunk* newVirtualChunk(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers], int size_factors[number_of_layers],
                                double layers_factors[number_of_layers]);


/**
 * @brief Generates a new chunk structure with a smooth transition with north and west chunks.
 * 
 * @param north_chunk (chunk*) : pointer to the chunk to the north.
 * @param west_chunk (chunk*) : pointer to the chunk to the west.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return chunk* : the pointer to the newly generated chunk structure.
 * 
 * @warning If both chunks are passed, both should have the same parameters (`width`, `height`, `number_of_layers` and `layers_factors`)
 * 
 * @note If you want to pass only a single chunk, pass `NULL` for the other pointer. You should not pass two `NULL` chunks though.
 */
chunk* newAdjacentChunk(chunk* north_chunk, chunk* west_chunk, unsigned int display_loading);



/**
 * @brief Makes a deep copy of the given chunk structure.
 * 
 * @param p_chunk (chunk*) : the pointer to the chunk to be copied.
 * @return chunk* : the pointer to the deep copy of the initial chunk.
 */
chunk* copyChunk(chunk* p_chunk);



/**
 * @brief Writes the chunk structure to a file at the given path.
 * 
 * @param chunk (chunk*) : pointer to the chunk to be written.
 * @param path (char[]) : the path to write the file to.
 */
void writeChunkFile(chunk* chunk, char path[]);

/**
 * @brief TODO : Reads the file at the given path and generates the associated chunk structure.
 * 
 * @param path (char[]) : the path to the file to be read.
 * @return chunk* : the pointer to the generated chunk structure.
 */
chunk* readChunkFile(char path[]);



/**
 * @brief Prints the chunk structure in the terminal.
 * 
 * @param chunk (chunk*) : the pointer to the chunk structure to be printed.
 */
void printChunk(chunk* chunk);

/**
 * @brief Frees the given chunk structure and every sub-structures.
 * 
 * @param chunk (chunk*) : the pointer to the chunk to be free'd.
 * 
 * @note Every layer structures composing the chunk will also be free'd.
 */
void freeChunk(chunk* chunk);

#endif
