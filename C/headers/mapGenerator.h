/**
 * @file mapGenerator.h
 * @author Zyno and BlueNZ
 * @brief Header to mapGenerator structure and functions
 * @version 0.3
 * @date 2024-07-31
 * 
 */

#ifndef MAP_GENERATOR
#define MAP_GENERATOR

#include "map.h"
#include "interpreter.h"

// ----- Structure definition -----

/**
 * @brief A color structure containing int and float parameters.
 * 
 */
struct color
{
    __uint8_t red;      /**< the red integer value in [0, 255]  */
    __uint8_t green;    /**< the green integer value in [0, 255]*/
    __uint8_t blue;     /**< the blue integer value in [0, 255] */
};

typedef struct color color;



/**
 * @brief A complete map structure containing an initial map, a sea_map and a color_map.
 * 
 */
struct completeMap
{
    map* map; /**< the initial map from which the complete map is generated.*/

    int width; /**< the width of the map*/
    int height; /**< the height of the map*/

    double sea_level; /**< the sea level of the complete map*/
    double* sea_values; /**< the array of altitude values of the sea_map where the minimum altitude is `sea_level`*/

    color** color_map; /**< the corresponding color map structure*/
};

typedef struct completeMap completeMap;

// ----- Functions -----

/**
 * @brief A Greatest Common Divisor algorithm, for positive numbers only.
 * 
 * @param a (int) : the first positive integer.
 * @param b (int) : the second positive integer.
 * @return int : the gcd of `a` and `b`.
 */
int gcd(int a, int b);

/**
 * @brief A Lowest Common Multiple algorithm, for positive numbers only.
 * 
 * @param a (int) : the first positive integer.
 * @param b (int) : the second positive integer.
 * @return int : the lcm of `a` and `b`.
 */
int lcm(int a, int b);

/**
 * @brief Finds and returns the lcm of the number in array.
 * 
 * @param nb (int) : number of integers to find the lcm of.
 * @param array (int[nb]) : the array of numbers to find the lcm of.
 * @return int : the lcm of the given array.
 */
int lcmOfArray(int nb, int array[nb]);



/**
 * @brief Get the pointer to the altitude value at the given indexes in the given completeMap structure.
 * 
 * @param completeMap (completeMap*) : pointer to the completeMap structure.
 * @param width_idx (int) : the width index of the altitude value pointer.
 * @param height_idx (int) : the height index of the altitude value pointer.
 * @return double* : the pointer to the wanted altitude value.
 */
double* getCompleteMapSeaValue(completeMap* completeMap, int width_idx, int height_idx);

/**
 * @brief Get the pointer to the color structure at the given indexes in the given completeMap structure.
 * 
 * @param completeMap (completeMap*) : pointer to the completeMap structure.
 * @param width_idx (int) : the width index of the color structure.
 * @param height_idx (int) : the height index of the color structure.
 * @return color* : the pointer to the wanted color structure.
 */
color* getCompleteMapColor(completeMap* completeMap, int width_idx, int height_idx);

/**
 * @brief Generates a new color structure from the given parameters and returns its pointer.
 * 
 * @param value (double) : the altitude value to generate the color from.
 * @param sea_level (double) : the sea level of the map.
 * @param min_value (double) : the minimum altitude value of the map.
 * @param max_value (double) : the maximum altitude value of the map.
 * @return color* : the pointer to the newly generated color.
 */
color* colorize(double value, double sea_level, double min_value, double max_value);



/**
 * @brief Generates a color_map from the given parameters.
 * 
 * @param map (map*) : pointer to the initial map structure.
 * @param sea_level (double) : sea altitude to use.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return color** : array of pointers to the generated color structures. Color map has the same dimensions as `map`.
 */
color** generateColorMap(map* map, double sea_level, unsigned int display_loading);

/**
 * @brief Generates a sea_map from the given parameters.
 * 
 * @param map (map*) : pointer to the initial map structure.
 * @param sea_level (double) : sea altitude to use.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return double* : array of altitude values where the sea if flat (the minimum value is sea_level). Sea map has the same dimensions as `map`.
 */
double* setSeaLevel(map* map, double sea_level, unsigned int display_loading);



/**
 * @brief Generates a new completeMap structure from the given map structure.
 * 
 * @param map (map*) : pointer to the map structure to generate the completeMap from.
 * @param sea_level (double) : sea altitude to use.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return completeMap* : pointer to the newly generated completeMap structure.
 */
completeMap* newCompleteMapFromMap(map* map, double sea_level, unsigned int display_loading);

/**
 * @brief Generates a new completeMap structure from scratch with the given parameters.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_width (int[number_of_layers]) : the array of gradientGrid width to be used to generate the random gradient grids.
 * @param gradGrids_height (int[number_of_layers]) : the array of gradientGrid height to be used to generate the random gradient grids.
 * @param size_factors (int[number_of_layers]) : the array of size factors to generate the layers.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param map_width (int) : number of chunks in width.
 * @param map_height (int) : number of chunks in height.
 * @param sea_level (double) : sea altitude to use.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return completeMap* : pointer to the newly generated completeMap structure.
 * 
 * @warning The `size_factors` array should match the `gradGrids_width` and `gradGrids_height` arrays such that
 *          `(gradGrid_dimensions - 1) * size_factor = constant`. In case it would not be the case, the chunk structure will still 
 *          be generated but its dimensions will be wrong : it will simply use the first value. 
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 */
completeMap* newCompleteMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                            int size_factors[number_of_layers], double layers_factors[number_of_layers],
                            int map_width, int map_height, double sea_level, unsigned int display_loading);



/**
 * @brief Generates a map structure with square chunks and automatic size factors.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_dimension (int[number_of_layers]) : the array of gradientGrid dimensions to be used to generate the random gradient grids.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param map_width (int) : number of chunks in width.
 * @param map_height (int) : number of chunks in height.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return map* : pointer to the newly generated map structure.
 * 
 * @note The arrays does not need to be dynamically allocated and their content will be copied in the structure.
 * 
 * @note The real gradient grids dimensions will be the passed ones + 1 to correctly match the passed dimensions' lcm to the final maps.
 */
map* get2dMap(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                    int map_width, int map_height, unsigned int display_loading);

//? Generate square chunks with automatic size factors and creates sea and color maps.
/**
 * @brief Generates a completeMap structure with square chunks, automatic size factors and the given sea altitude.
 * It consists in generating a new map with the `get2dMap` function, and then creating a new completeMap with the `newCompleteMapFromMap` function.
 * 
 * @param number_of_layers (int) : the number of layers passed.
 * @param gradGrids_dimension (int[number_of_layers]) : the array of gradientGrid dimensions to be used to generate the random gradient grids.
 * @param layers_factors (double[number_of_layers]) : the array of layers factors.
 * @param map_width (int) : number of chunks in width.
 * @param map_height (int) : number of chunks in height.
 * @param sea_level (double) : sea altitude to use.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return completeMap* : pointer to the newly generated completeMap structure.
 */
completeMap* fullGen(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                         int map_width, int map_height, double sea_level, unsigned int display_loading);



/**
 * @brief TODO : Prints the given completeMap in the terminal.
 * 
 * @param completeMap (completeMap*) : pointer to the completeMap structure to be printed.
 */
void printCompleteMap(completeMap* completeMap);





bytes bytesColor(color c);

tuple_obj_bytes nextColor(bytes bytes);

bytes bytesCompleteMap(completeMap* cmap);

tuple_obj_bytes nextCompleteMap(bytes bytes);



/**
 * @brief Writes the sea_map in a file at the given path.
 * 
 * @param completeMap (completeMap*) : the pointer to the completeMap structure containing the sea map to be written.
 * @param path (char[]) : path of the file to be written.
 */
void writeSeaMapFile(completeMap* completeMap, char path[]);

/**
 * @brief Writes the color_map in a file at the given path.
 * 
 * @param width (int) : the width of the color map
 * @param height (int) : the height of the color map.
 * @param color_map (color**) : the array of pointers to the color structures representing the color map.
 * @param path (char[]) : the path where the file shall be written.
 */
void writeColorMapFile(int width, int height, color* color_map[width * height], char path[]);

/**
 * @brief Writes every required files to save the completeMap structure.
 * 
 * @param complete_map (completeMap*) : the pointer to the colorMap to be saved.
 * @param path (char[]) : the path to the folder where the files shall be written.
 */
void writeCompleteMapFiles(completeMap* complete_map, char path[]);

/**
 * @brief TODO : Generates the completeMap structure saved in the given file.
 * 
 * @param path (char[]) : path to the file to be read.
 * @return completeMap* : the pointer to the newly generated completeMap structure.
 */
completeMap* readCompleteMapFiles(char path[]);



/**
 * @brief Frees the given completeMap structure and every sub-structures contained.
 * 
 * @param completeMap (completeMap*) : the pointer to the completeMap structure to be free'd.
 * 
 * @note The initial map from which the completeMap was generated will also be free'd.
 */
void freeCompleteMap(completeMap* completeMap);

#endif
