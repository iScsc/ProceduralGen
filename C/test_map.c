#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"

int main()
{
    int display_loading = 1;

    int nb_layers = 2;

    int gradGrids_width[2] = {2+1, 7+1};
    int gradGrids_height[2] = {2+1, 7+1};
    int size_factors[2] = {7, 2};

    double* layers_factors = calloc(nb_layers, sizeof(double));
    layers_factors[0] = 1;
    layers_factors[1] = 0.1;

    int map_width = 3;
    int map_height = 2;

    printf("Creating a full map...\n");

    map* my_map = newMap(nb_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, map_width, map_height, display_loading);

    printMap(my_map);

    printf("Deallocating now...\n");

    freeMap(my_map);

    return 0;
}
