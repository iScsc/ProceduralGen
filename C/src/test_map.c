/**
 * @file test_map.c
 * @author Zyno
 * @brief a testing script for the map implementation
 * @version 0.1
 * @date 2024-05-21
 * 
 */

#include <stdio.h>
#include <time.h>

#include "loadingBar.h"
#include "gradientGrid.h"
#include "layer.h"
#include "chunk.h"
#include "map.h"

int main()
{
    setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

    int display_loading = 1;

    int nb_layers = 2;

    int gradGrids_width[2] = {2+1, 7+1};
    int gradGrids_height[2] = {2+1, 7+1};
    int size_factors[2] = {7, 2};

    double layers_factors[2] = {1, .1};

    int map_width = 3;
    int map_height = 2;

    printf("Creating a full map...\n");

    map* my_map = newMap(nb_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, map_width, map_height, display_loading);

    printMap(my_map);



    //? Comment this if you don't want to save it in a file.
    //! WARNING : ../saves/ the folder must exist for it to work properly
    printf("File creation...\n");
    char path[200] = "../saves/map_test.txt";

    writeMapFile(my_map, path);
    printf("File should be written now.\n");


    printf("Deallocating now...\n");

    freeMap(my_map);

    return 0;
}
