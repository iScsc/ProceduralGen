/**
 * @file layer.h
 * @author Zyno and BlueNZ
 * @brief Header to layer structure and functions
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#ifndef LAYER
#define LAYER

#include <stdbool.h>

#include "gradientGrid.h"
#include "interpreter.h"

// ----- Structure definition -----

/**
 * @brief A layer structure.
 * 
 */
struct layer
{
    int width; /**< the width of the layer*/
    int height; /**< the height of the layer*/
    int size_factor; /**< the size factor used to pass from the gradientGrid dimensions to this layer's dimensions*/

    gradientGrid* gradient_grid; /**!< the pointer to the gradientGrid structure used to generate this layer*/

    double* values; /**!< the array of altitude values*/
};

typedef struct layer layer;

// ----- Functions -----

/**
 * @brief A smoothstep function to smoothly pass from 0 to 1.
 * It uses the function 3w² - 2w³
 * 
 * @param w (double) : the input value
 * @return double : the resulting smoothstep value
 */
double smoothstep(double w);

/**
 * @brief A smooth interpolating function.
 * 
 * @param a0 (double) : the first value to interpolate from.
 * @param a1 (double) : the second value to interpolate from.
 * @param w (double) : the smoothstep input.
 * @return double : the interpolated value between `a0` and `a1` using the smoothstep input `w`.
 */
double interpolate(double a0, double a1, double w);



/**
 * @brief Computes the dot product between the (x - ix, y - iy) vector and the extracted vector from the given gradient grid.
 * 
 * @param ix (int) : the width index to extract the vector from the gradient grid.
 * @param iy (int) : the height index to extract the vector from the gradient grid.
 * @param x (double) : the width position.
 * @param y (double) : the height position.
 * @param gradient_grid (gradientGrid*) : the pointer to the gradientGrid structure.
 * @return double : the computed dot product.
 */
double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradient_grid);

/**
 * @brief Computes the altitude value at position (x, y) from the given gradient grid.
 * 
 * @param x (double) : the width position.
 * @param y (double) : the height position.
 * @param gradient_grid (gradientGrid*) : the pointer to the gradientGrid structure.
 * @return double : the final altitude value.
 */
double perlin(double x, double y, gradientGrid* gradient_grid);



/**
 * @brief Get the layer altitude pointer at given indexes.
 * 
 * @param layer (layer*) : the pointer to the layer structure.
 * @param width_idx (int) : the width index.
 * @param height_idx (int) : the height index.
 * @return double* : the pointer to the corresponding altitude value.
 */
double* getLayerValue(layer* layer, int width_idx, int height_idx);



/**
 * @brief Generates a new layer structure from the given gradientGrid and parameters.
 * 
 * @param gradient_grid (gradientGrid*) : the pointer to the gradientGrid structure to build the layer from.
 * @param size_factor (int) : the size_factor to rescale the layer's dimensions.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * 
 * @return layer* : the pointer to the newly created layer structure.
 * 
 * @note Please be aware that the layer's dimensions are `(gradGrid_dimensions - 1) * size_factor`
 */
layer* newLayerFromGradient(gradientGrid* gradient_grid, int size_factor, unsigned int display_loading);

/**
 * @brief Generates a new layer structure from scratch with the given parameters.
 * 
 * @param gradGrid_width (int) : the width of the gradient grid to generate.
 * @param gradGrid_height (int) : the height of the gradient grid to generate.
 * @param size_factor (int) : the scaling factor to pass to the layer's dimensions.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return layer* : the pointer to the newly created layer structure.
 * 
 * @note Please be aware that the layer's dimensions are `(gradGrid_dimensions - 1) * size_factor`
 */
layer* newLayer(int gradGrid_width, int gradGrid_height, int size_factor, unsigned int display_loading);



/**
 * @brief Makes a deep copy of the given layer structure.
 * 
 * @param p_layer (layer*) : the pointer to the layer to be copied.
 * @return layer* : the pointer to the deep copy of the initial layer.
 */
layer* copyLayer(layer * p_layer);




bytes bytesLayer(layer* grid);

tuple_obj_bytes nextLayer(bytes bytes);



/**
 * @brief Writes the layer structure to a file at the given path.
 * 
 * @param layer (layer*) : the pointer to the layer structure to be written.
 * @param path (char[]) : the path to write the file to.
 */
void writeLayerFile(layer* layer, char path[]);

/**
 * @brief TODO : Reads a layer structure from a file.
 * 
 * @param path (char[]) : path to the file to be read.
 * @return layer* : the pointer to the newly generated layer structure.
 */
layer* readLayerFile(char path[]);



/**
 * @brief Prints the layer structure in the terminal.
 * 
 * @param layer (layer*) : pointer to the layer structure to be printed.
 */
void printLayer(layer* layer);

/**
 * @brief Frees the given layer structure and every sub-structure.
 * 
 * @param layer (layer*) : pointer to the layer to be free'd.
 * 
 * @note the linked gradientGrid will also be correctly free'd and requires no user treatment.
 */
void freeLayer(layer* layer);

#endif
