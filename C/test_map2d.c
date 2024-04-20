#include <stdio.h>
#include <time.h>

#include "gradientGrid.h"
#include "map2d.h"

int main()
{
    int width = 7;
    int height = 5;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating a gradient grid of size (height x width) = (%d x %d)\n", height, width);

    gradientGrid* gradGrid = newRandomGradGrid(width, height);

    printGradientGrid(gradGrid);



    int sizeFactor = 3;

    printf("Creating a 2d map of size (height x width) = (%d x %d)\n", sizeFactor*(height-1), sizeFactor*(width-1));

    map2d* map = newMap2d(gradGrid, sizeFactor);

    printMap2d(map);



    printf("Deallocating now...\n");

    freeGradGrid(gradGrid);

    freeMap2d(map);

    return 0;
}
