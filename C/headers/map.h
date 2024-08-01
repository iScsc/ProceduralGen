/**
 * @file map.h
 * @author Zyno and BlueNZ
 * @brief Header to map structure and functions
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#ifndef MAP
#define MAP

#include "chunk.h"
#include "interpreter.h"

// ----- Structure definition -----

/**
 * @brief A map structure.
 * 
 */
struct map
{
    int map_width; /**< the number of chunks in width*/
    int map_height; /**< the number of chunks in height*/
    chunk** chunks; /**< the array of pointers to the corresponding chunk structures*/

    /**< the array of virtual chunks needed for border conditions and future map expansion.
    *Stored in order (Col0,ColN,Row0,RowN) without repetition of the corners
    *Its size is `(map_height+2 + map_width+2)*2 - 4`*/
    chunk** virtual_chunks;

    int chunk_width; /**< the width of each chunk*/
    int chunk_height; /**< the height of each chunk*/
    // double* map_values; /**< the array of altitude values. Its size is `map_width * chunk_width` x `map_height * chunk_height`*/
};

typedef struct map map;

// ----- Functions -----

/**
 * @brief Get the pointer to the altitude value at given width and height indexes in the given map structure.
 * This points to the altitude value in the correct chunk.
 * 
 * @param map (map*) : the pointer to the map structure.
 * @param width_idx (int) : the width index of the wanted value.
 * @param height_idx (int) : the height index of the wanted value.
 * @return double* : the pointer to the map altitude value in the correct chunk structure.
 */
double* getMapValue(map* map, int width_idx, int height_idx);

/**
 * @brief Get the Chunk structure at given indexes.
 * 
 * @param map (map*) : the pointer to the map structure.
 * @param width_idx (int) : the width index of the wanted chunk.
 * @param height_idx (int) : the height index of the wanted chunk.
 * @return chunk* : the pointer to the corresponding chunk structure.
 */
chunk* getChunk(map* map, int width_idx, int height_idx);

/**
 * @brief Get the virtual Chunk structure at given indexes.
 * One of the given idx must be `0` or the `corresponding dimension - 1`.
 * It returns `null` if the given idx pair is invalid.
 * 
 * @param map (map*) : the pointer to the map structure.
 * @param width_idx (int) : the width index of the wanted virtual chunk.
 * @param height_idx (int) : the height index of the wanted virtual chunk.
 * @return chunk* : the pointer to the corresponding virtual chunk structure.
 */
chunk* getVirtualChunk(map* map, int width_idx, int height_idx);



/**
 * @brief Get the full map altitude values built from concatenation of each chunk altitude value.
 * 
 * @param map (map*) : the pointer to the map to get the full map altitude from.
 * @return double* : the pointer to the full altitude array. It is of size 
 * `map.map_width * map.chunk_width * map.map_height * map.chunk_height`.
 */
double* getFullMap(map* map);



/**
 * @brief Interpolates the given values in 2d in order to get a smooth 2d interpolation.
 * 
 * @param a1 (double) : the first altitude value, located in `(0, 0)`.
 * @param a2 (double) : the second altitude value, located in `(1, 0)`.
 * @param a3 (double) : the third altitude value, located in `(0, 1)`.
 * @param a4 (double) : the fourth altitude value, located in `(1, 1)`.
 * @param x (double) : the relative x coordinate from the four adjacent chunks. Should be in `[0, 1]`.
 * @param y (double) : the relative y coordinate from the four adjacent chunks. Should be in `[0, 1]`.
 * @return double : the interpolated final value.
 */
double interpolate2D(double a1, double a2, double a3, double a4, double x, double y);

/**
 * @brief Modifies the chunks of the given map to process each chunk's base altitude and adapt the final altitude values of each chunk.
 * 
 * @param p_map (map*) : the pointer to the original untreated map.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return map* : the pointer to the treated map, which is exactly the same as the given `p_map`.
 */
map* addMeanAltitude(map* p_map, unsigned int display_loading);



/**
 * @brief Creates a new map structure from the given array of chunks.
 * 
 * @param map_width (int) : number of chunks in width.
 * @param map_height (int) : number of chunks in height.
 * @param chunks (chunk**) : array of size `map_width * map_height` of pointers to the chunk structures.
 * @param virtual_chunks (chunk**) : array of size `(map_height+2 + map_width+2)*2 - 4` of pointers to the virtual chunk structures.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return map* : the pointer to the newly generated map structure.
 * 
 * @note The chunks array does not need to be dynamically allocated and its content will be copied in the structure.
 */
map* newMapFromChunks(int map_width, int map_height, chunk* chunks[map_width * map_height], chunk* virtual_chunks[(map_height+2+map_width+2)*2-4], bool regenerate, unsigned int display_loading);

/**
 * @brief Creates a new map from scratch with the given parameters.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_width (int[number_of_layers]) : the array of gradientGrid width to be used to generate the random gradient grids.
 * @param gradGrids_height (int[number_of_layers]) : the array of gradientGrid height to be used to generate the random gradient grids.
 * @param size_factors (int[number_of_layers]) : the array of size factors to generate the layers.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param map_width (int) : number of chunks in width.
 * @param map_height (int) : number of chunks in height.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return map* : the pointer to the newly generated map structure.
 * 
 * @warning The `size_factors` array should match the `gradGrids_width` and `gradGrids_height` arrays such that
 *          `(gradGrid_dimensions - 1) * size_factor = constant`. In case it would not be the case, the chunk structure will still 
 *          be generated but its dimensions will be wrong : it will simply use the first value. 
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
map* newMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                 int size_factors[number_of_layers], double layers_factors[number_of_layers], int map_width, int map_height, unsigned int display_loading);



/**
 * @brief Makes a deep copy of the given map structure.
 * 
 * @param p_map (map*) : the pointer to the map to be copied.
 * @return map* : the pointer to the deep copy of the initial map.
 */
map* copyMap(map* p_map) ;



/**
 * @brief Encodes a map struct in a binary format.
 * 
 * @param map (map*) : a pointer to the map struct.
 * @return bytes : the byte string representing the encoded struct.
 */
bytes bytesMap(map* map);

/**
 * @brief Decodes a map struct from a formatted byte string.
 * 
 * @param bytes (bytes) : the formatted byte string.
 * @return tuple_obj_bytes : the decoded map and the byte string (with the start index updated).
 */
tuple_obj_bytes nextMap(bytes bytes);



/**
 * @brief Writes the map structure into a file at the given path.
 * 
 * @param map (map*) : pointer to the map structure to write.
 * @param path (char[]) : writing path for the file.
 */
void writeMapFile(map* map, char path[]);

/**
 * @brief TODO : Reads the given file and generates the associated map structure.
 * 
 * @param path (char[]) : path to the file to be read.
 * @return map* : the pointer to the generated map structure.
 */
map* readMapFile(char path[]);



/**
 * @brief Prints the given map structure in the terminal.
 * 
 * @param map (map*) : the pointer to the map structure to be printed.
 */
void printMap(map* map);

/**
 * @brief Frees the given map structure and every sub-structures contained.
 * 
 * @param map (map*) : the pointer to the map structure to be free'd.
 * 
 * @note Every chunk structures composing the chunk will also be free'd.
 */
void freeMap(map* map);

#endif
