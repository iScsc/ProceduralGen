#ifndef GRADIENT_GRID
#define GRADIENT_GRID

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

gradientGrid* newGradGrid(int width, int height);

gradientGrid* newRandomGradGrid(int width, int height);

void regenerateRandomGradGrid(gradientGrid* p_gradGrid);

vector* getVector(gradientGrid* p_gragGrid, int width_idw, int height_idx);

void printGradientGrid(gradientGrid* p_gradGrid);

void freeGradGrid(gradientGrid* p_gradGrid);

#endif
