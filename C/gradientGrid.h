#ifndef GRADIENT_GRID
#define GRADIENT_GRID

// ----- Constants definition -----

#define NUMBER_OF_SEGMENTS 21

// ----- Structure definition -----

struct vector
{
    double x;
    double y;
};

typedef struct vector vector;

struct gradientGrid
{
    int width;
    int height;
    vector* gradients;
};

typedef struct gradientGrid gradientGrid;

// ----- Functions -----

void setRandomSeed(unsigned int seed);

vector* getVector(gradientGrid* p_gradGrid, int width_idx, int height_idx);



void regenerateRandomGradGrid(gradientGrid* p_gradGrid, int display_loading);



gradientGrid* newGradGrid(int width, int height);

gradientGrid* newRandomGradGrid(int width, int height, int display_loading);



gradientGrid* newAdjacentGradGrid(gradientGrid* north_grid, gradientGrid* west_grid, int display_loading);



void printGradientGrid(gradientGrid* p_gradGrid);

void freeGradGrid(gradientGrid* p_gradGrid);

#endif
