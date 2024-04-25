#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"

int main()
{
    int display_loading = 1;

    int width1 = 3;
    int height1 = 3;

    int width2 = 7;
    int height2 = 7;

    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    printf("Creating two gradient grids of sizes (height, width) = (%d x %d) and (%d x %d)\n", height1, width1, height2, width2);

    gradientGrid* gradGrid1 = newRandomGradGrid(width1 + 1, height1 + 1, display_loading);
    printGradientGrid(gradGrid1);

    gradientGrid* gradGrid2 = newRandomGradGrid(width2 + 1, height2 + 1, display_loading);
    printGradientGrid(gradGrid2);



    int sizeFactor1 = 7;
    int sizeFactor2 = 3;


    int layer_width1 = sizeFactor1*width1;
    int layer_height1 = sizeFactor1*height1;

    int layer_width2 = sizeFactor2*width2;
    int layer_height2 = sizeFactor2*height2;

    printf("Creating two layers of sizes (height x width) = (%d x %d) and (%d x %d)\n",
                                                    layer_height1, layer_width1,
                                                    layer_height2, layer_width2);

    layer* layer1 = newLayerFromGradient(gradGrid1, sizeFactor1, display_loading);
    printLayer(layer1);
    
    layer* layer2 = newLayerFromGradient(gradGrid2, sizeFactor2, display_loading);
    printLayer(layer2);



    double* my_factors = calloc(2, sizeof(double));
    my_factors[0] = 1;
    my_factors[1] = 0.01;

    layer** my_layers = calloc(2, sizeof(layer*));
    my_layers[0] = layer1;
    my_layers[1] = layer2;

    printf("Creating chunk with these two layers, and factors = {%lf, %lf}\n", my_factors[0], my_factors[1]);

    chunk* my_chunk = newChunkFromLayers(layer_width1, layer_height1, 2, my_factors, my_layers, display_loading);

    printChunk(my_chunk);

    printf("Deallocating now...\n");

    freeChunk(my_chunk);

    return 0;
}
