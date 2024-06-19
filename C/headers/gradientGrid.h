/**
 * @file gradientGrid.h
 * @author Zyno and BlueNZ
 * @brief Header to gradient grid structure and functions
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#ifndef GRADIENT_GRID
#define GRADIENT_GRID

// ----- Structure definition -----

/**
 * @brief A 2d vector structure.
 * 
 */
struct vector
{
    double x; /**< the x coordinate*/
    double y; /**< the y coordinate*/
};

typedef struct vector vector;

/**
 * @brief A 2d gradient vector array.
 * 
 */
struct gradientGrid
{
    int width; /**< the width of the gradientGrid*/
    int height; /**< the height of the gradientGrid*/
    vector* gradients; /**< the pointer to the array of vectors*/
};

typedef struct gradientGrid gradientGrid;

// ----- Functions -----

/**
 * @brief Sets the Random Seed to the given unsigned int.
 * 
 * @param seed (unsigned int) : the seed to setup the random generator.
 */
void setRandomSeed(unsigned int seed);

/**
 * @brief Gets the Vector from gradGrid at given indexes.
 * 
 * @param gradGrid (gradientGrid*) : the pointer to the corresponding gradient grid.
 * @param width_idx (int) : the width index of the wanted vector.
 * @param height_idx (int) : the height index of the wanted vector.
 * @return vector* : the vector at given indexes
 */
vector* getVector(gradientGrid* gradGrid, int width_idx, int height_idx);



/**
 * @brief Regenerates random vectors in the gradient grid.
 * 
 * @param gradGrid (gradientGrid*) : the pointer to the corresponding gradient grid.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 */
void regenerateRandomGradGrid(gradientGrid* gradGrid, unsigned int display_loading);



/**
 * @brief Generates a new gradientGrid object with given width and height.
 * Be aware that the vectors won't be initialized to any value, but the memory space will be calloc'd.
 * 
 * @param width (int) : the width of the generated gradient grid.
 * @param height (int) : the height of the generated gradient grid.
 * @return gradientGrid* : the pointer to the generated gradient grid.
 */
gradientGrid* newGradGrid(int width, int height);

/**
 * @brief Generates a new gradientGrid object with given width and height initialized to random vectors.
 * 
 * @param width (int) : the width of the generated gradient grid.
 * @param height (int) : the height of the generated gradient grid.
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return gradientGrid* : the pointer to the generated gradient grid.
 */
gradientGrid* newRandomGradGrid(int width, int height, unsigned int display_loading);



//TODO: Extends this definition to make the function able to expand the map on the north and west directions.
/**
 * @brief Generates a new random gradientGrid with boundary values set to the corresponding adjacent grids'.
 * 
 * @param north_grid (gradientGrid*) : the pointer to the gradientGrid located at the north (height index < 0)
 * @param west_grid (gradientGrid*) : the pointer to the gradientGrid located at the west (width index < 0)
 * @param display_loading (unsigned int) : the given value defines the behaviour.
 *                                         * If `0` the loading bars won't be printed.
 *                                         * If `> 0` the loading bars will be printed with a number of indent equal to `display_loading - 1`.
 * @return gradientGrid* : the pointer to the generated gradient grid.
 */
gradientGrid* newAdjacentGradGrid(gradientGrid* north_grid, gradientGrid* west_grid, unsigned int display_loading);



/**
 * @brief Makes a deep copy of the given gradientGrid structure.
 * 
 * @param grad (gradientGrid*) : the pointer to the gradientGrid to be copied.
 * @return gradientGrid* : the pointer to the deep copy of the initial gradientGrid.
 */
gradientGrid* copyGrad(gradientGrid* grad);

/**
 * @brief Makes a copy of the given vector structure.
 * 
 * @param vect (vector*) : the pointer to the vector to be copied.
 * @return vector* : the pointer to the copy of the initial vector.
 */
vector* copyVect(vector* vect);



/**
 * @brief Writes the given gradientGrid in a new file at path. If a file already exists, it will be overwritten.
 * 
 * @param gradGrid (gradientGrid*) : the pointer to the corresponding gradient grid.
 * @param path (char[]) : the path to the file to write.
 */
void writeGradientGridFile(gradientGrid* gradGrid, char path[]);

/**
 * @brief TODO : Reads a gradientGrid file and generates the corresponding gradient grid.
 * 
 * @param path (char[]) : the path to the file to read.
 * @return gradientGrid* : the pointer to the generated gradient grid.
 */
gradientGrid* readGradientGridFile(char path[]);



/**
 * @brief Prints in the terminal the given gradient grid.
 * 
 * @param gradGrid (gradientGrid*) : the pointer to the corresponding gradient grid.
 */
void printGradientGrid(gradientGrid* gradGrid);

/**
 * @brief Frees the given gradientGrid structure and every sub-structures.
 * 
 * @param gradGrid (gradientGrid*) : the pointer to the corresponding gradient grid.
 */
void freeGradGrid(gradientGrid* gradGrid);

#endif
