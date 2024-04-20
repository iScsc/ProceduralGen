#include <stdio.h>

#include "gradientGrid.h"

int main()
{
    int width = 10;
    int height = 7;

    printf("Creating a gradient grid of size (width x height) = (%d x %d)\n", width, height);

    gradientGrid* gradGrid = newGradGrid(width, height);

    printf("Total space used by this gradient grid : %ld\n", sizeof(*gradGrid) + width*height*sizeof(*(gradGrid->gradients)));
    //                                              Should be :    4 + 4 + 8   + width*height    *      8 + 8

    printf("Deallocating now...\n");

    freeGradGrid(gradGrid);

    return 0;
}
