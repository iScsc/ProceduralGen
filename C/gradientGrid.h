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

vector* getVector(gradientGrid* gradGrid, int width_idx, int height_idx);



void regenerateRandomGradGrid(gradientGrid* gradGrid, unsigned int display_loading);



gradientGrid* newGradGrid(int width, int height);

gradientGrid* newRandomGradGrid(int width, int height, unsigned int display_loading);



gradientGrid* newAdjacentGradGrid(gradientGrid* north_grid, gradientGrid* west_grid, unsigned int display_loading);



void writeGradientGridFile(gradientGrid* gradGrid, char path[]);

gradientGrid* readGradientGridFile(char path[]);



void printGradientGrid(gradientGrid* gradGrid);

void freeGradGrid(gradientGrid* gradGrid);

#endif
