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

    printf("Creating adjacent East gradient grid :\n");
    gradientGrid* gradientEast = newAdjacentGradGrid(NULL, gradGrid, display_loading);
    printGradientGrid(gradientEast);


    printf("Creating adjacent South gradient grid :\n");
    gradientGrid* gradientSouth = newAdjacentGradGrid(gradGrid, NULL, display_loading);
    printGradientGrid(gradientSouth);


    printf("Creating adjacent South-East gradient grid :\n");
    gradientGrid* gradientSouthEast = newAdjacentGradGrid(gradientEast, gradientSouth, display_loading);
    printGradientGrid(gradientSouthEast);


    printf("File creation...\n");
    char path[200] = "../saves/gradGrid_test.txt";

    writeGradientGridFile(gradGrid, path);
    printf("File should be written now.\n");


    printf("Deallocating now...\n");

    freeGradGrid(gradGrid);
    freeGradGrid(gradientEast);
    freeGradGrid(gradientSouth);
    freeGradGrid(gradientSouthEast);

    return 0;
}
