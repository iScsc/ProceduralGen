#include <stdio.h>
#include <time.h>

#include "gradientGrid.h"
#include "layer.h"

int main()
{
    int display_loading = 1;

    int width = 21;
    int height = 11;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating a gradient grid of size (height x width) = (%d x %d)\n", height, width);

    gradientGrid* gradGrid = newRandomGradGrid(width, height, display_loading);

    printGradientGrid(gradGrid);



    int sizeFactor = 10;

    printf("Creating a layer of size (height x width) = (%d x %d)\n", sizeFactor*(height-1), sizeFactor*(width-1));

    layer* layer1 = newLayerFromGradient(gradGrid, sizeFactor, display_loading);

    printLayer(layer1);



    printf("Trying another layer.\n");

    layer* another_layer = newLayer(width, height, sizeFactor, display_loading);

    printLayer(another_layer);



    printf("Deallocating now...\n");

    freeLayer(layer1);

    freeLayer(another_layer);

    return 0;
}
