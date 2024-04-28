#ifndef MAP_GENERATOR
#define MAP_GENERATOR

#include "map.h"

// ----- Structure definition -----

//? Maybe to be added to map.h?
struct color
{
    int red_int;
    int green_int;
    int blue_int;

    double red_double;
    double green_double;
    double blue_double;
};

typedef struct color color;



struct completeMap
{
    map* map;

    int width;
    int height;

    double sea_level;
    double* sea_values;

    color** color_map;
};

typedef struct completeMap completeMap;

// ----- Functions -----

//? For positive integers only. Should not be an issue here.
int gcd(int a, int b);

int lcm(int a, int b);

int lcmOfArray(int nb, int array[nb]);



color* getCompleteMapColor(completeMap* completeMap, int width_idx, int height_idx);

color* colorize(double value, double sea_level, double min_value, double max_value);



color** generateColorMap(map* map, double sea_level, int display_loading);

double* setSeaLevel(map* map, double sea_level, int display_loading);



completeMap* newCompleteMapFromMap(map* map, double sea_level, int display_loading);

completeMap* newCompleteMap(int number_of_layers, int* gradGrids_width, int* gradGrids_height, int* size_factors, double* layers_factors,
                            int map_width, int map_height, double sea_level, int display_loading);



//? Generate square chunks with automatic size factors.
map* get2dMap(int number_of_layers, int* gradGrids_dimension, double* layers_factors, int map_width, int map_height, int display_loading);

//? Generate square chunks with automatic size factors and creates sea and color maps.
completeMap* fullGen(int number_of_layers, int* gradGrids_dimension, double* layers_factors, int map_width, int map_height, double sea_level,
                        int display_loading);



void printCompleteMap(completeMap* completeMap);



void saveColorMap(char* filepath);



void freeCompleteMap(completeMap* completeMap);

#endif