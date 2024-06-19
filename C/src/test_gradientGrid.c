/**
 * @file test_gradientGrid.c
 * @author Zyno
 * @brief a testing script for the gradientGrid implementation
 * @version 0.1
 * @date 2024-05-07
 * 
 */

#include <stdio.h>
#include <time.h>

#include "gradientGrid.h"

int main()
{
    int display_loading = 1; // Means no base indent in prints

    int width = 7;
    int height = 5;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating a gradient grid of size (width x height) = (%d x %d)\n", width, height);

    gradientGrid* gradGrid = newRandomGradGrid(width, height, display_loading);

    printf("Total space used by this gradient grid : %ld\n", sizeof(*gradGrid) + width*height*sizeof(*(gradGrid->gradients)));
    //                                              Should be :    4 + 4 + 8   + width*height    *      (8 + 8)


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


    //? Comment this if you don't want to save it in a file.
    //! WARNING : ../saves/ the folder must exist for it to work properly
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
