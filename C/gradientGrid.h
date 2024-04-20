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

gradientGrid* newGradGrid(int width, int height);

void freeGradGrid(gradientGrid* p_gradGrid);

#endif
