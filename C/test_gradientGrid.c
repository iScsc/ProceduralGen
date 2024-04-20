#include <stdio.h>
#include <time.h>

#include "gradientGrid.h"

int main()
{
    int width = 7;
    int height = 5;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating a gradient grid of size (width x height) = (%d x %d)\n", width, height);

    gradientGrid* gradGrid = newRandomGradGrid(width, height, 1);

    printf("Total space used by this gradient grid : %ld\n", sizeof(*gradGrid) + width*height*sizeof(*(gradGrid->gradients)));
    //                                              Should be :    4 + 4 + 8   + width*height    *      8 + 8


    printGradientGrid(gradGrid);

    printf("Deallocating now...\n");

    freeGradGrid(gradGrid);

    return 0;
}
