#include <stdio.h>
#include <time.h>

#include "gradientGrid.h"

int main()
{
    int display_loading = 1;

    int width = 7;
    int height = 5;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating a gradient grid of size (width x height) = (%d x %d)\n", width, height);

    gradientGrid* gradGrid = newRandomGradGrid(width, height, display_loading);

    printf("Total space used by this gradient grid : %ld\n", sizeof(*gradGrid) + width*height*sizeof(*(gradGrid->gradients)));
    //                                              Should be :    4 + 4 + 8   + width*height    *      8 + 8


    printGradientGrid(gradGrid);

    printf("Creating adjacent Est gradient grid :\n");
    gradientGrid* gradientEst = newAdjacentGradGrid(NULL, gradGrid, display_loading);
    printGradientGrid(gradientEst);


    printf("Creating adjacent South gradient grid :\n");
    gradientGrid* gradientSouth = newAdjacentGradGrid(gradGrid, NULL, display_loading);
    printGradientGrid(gradientSouth);


    printf("Creating adjacent South-Est gradient grid :\n");
    gradientGrid* gradientSouthEst = newAdjacentGradGrid(gradientEst, gradientSouth, display_loading);
    printGradientGrid(gradientSouthEst);

    printf("Deallocating now...\n");

    freeGradGrid(gradGrid);
    freeGradGrid(gradientEst);
    freeGradGrid(gradientSouth);
    freeGradGrid(gradientSouthEst);

    return 0;
}
