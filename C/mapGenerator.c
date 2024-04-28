#include <malloc.h>

#include "loadingBar.h"
#include "map.h"
#include "mapGenerator.h"

//? For positive integers only. Should not be an issue here.
int gcd(int a, int b)
{
    if (a == 0 || b == 0)
    {
        printf("%sERROR : can't find the greatest common divisor of 0 !%s\n", RED_COLOR, DEFAULT_COLOR);
        return 0;
    }

    while (a != b)
    {
        if (a > b)
        {
            a -= b;
        }
        else
        {
            b -= a;
        }
    }

    return a;
}



int lcm(int a, int b)
{
    if (a == 0 || b == 0)
    {
        return 0;
    }

    int c = gcd(a, b);

    return (a * b) / c;
}



int lcmOfArray(int nb, int array[nb])
{
    int common = 0;

    if (array != NULL && nb > 1)
    {
        common = lcm(array[0], array[1]);

        for (int i = 2; i < nb; i++)
        {
            common = lcm(common, array[i]);
        }
    }

    return common;
}





color* getCompleteMapColor(completeMap* completeMap, int width_idx, int height_idx)
{
    color* map_color = NULL;

    if (completeMap != NULL)
    {
        int width = completeMap->width;
        int height = completeMap->height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading complete map color. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading complete map color. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }

        map_color = completeMap->color_map[height_idx * width + width_idx];
    }

    return map_color;
}



color* colorize(double value, double sea_level, double min_value, double max_value)
{
    color* new_color = calloc(1, sizeof(color));

    int i = (int) ((value - min_value) * 255. / (max_value - min_value));
    int s = (int) ((value - min_value) * 200. / (max_value - min_value));

    //? Show red dots on zero values.
    if (value == 0)
    {
        new_color->red_int = 255;
        new_color->green_int = 0;
        new_color->blue_int = 0;

        new_color->red_double = 1.;
        new_color->green_double = 0;
        new_color->blue_double = 0;
    }
    else if (value < sea_level)
    {
        new_color->red_int = 50;
        new_color->green_int = 50;
        new_color->blue_int = s;

        new_color->red_double = 1./255 * 50;
        new_color->green_double = 1./255 * 50;
        new_color->blue_double = 1./255 * s;
    }
    else
    {
        new_color->red_int = 50;
        new_color->green_int = i;
        new_color->blue_int = 50;

        new_color->red_double = 1./255 * 50;
        new_color->green_double = 1./255 * i;
        new_color->blue_double = 1./255 * 50;
    }

    return new_color;
}





color** generateColorMap(map* map, double sea_level, int display_loading)
{
    color** color_map = NULL;

    if (map != NULL)
    {
        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        color_map = calloc(width * height, sizeof(color*));
        if (color_map == NULL)
        {
            printf("%sColor map allocation was not successful. Returning NULL now...%s\n", RED_COLOR, DEFAULT_COLOR);
            return NULL;
        }

        double min_value = *getMapValue(map, 0, 0);
        double max_value = *getMapValue(map, 0, 0);

        // Getting min and max values.


        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double map_value = *getMapValue(map, j, i);

                if (map_value < min_value)
                {
                    min_value = map_value;
                }

                if (map_value > max_value)
                {
                    max_value = map_value;
                }
            }
        }


        // Generating colors

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double map_value = *getMapValue(map, j, i);

                color* c = colorize(map_value, sea_level, min_value, max_value);
                
                color_map[i * width + j] = c;
            }
        }
    }

    return color_map;
}



double* setSeaLevel(map* map, double sea_level, int display_loading)
{
    double* sea_map = NULL;

    if (map != NULL)
    {
        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        sea_map = calloc(width * height, sizeof(double));
        if (sea_map == NULL)
        {
            printf("%sSea map allocation was not successful. Returning NULL now...%s\n", RED_COLOR, DEFAULT_COLOR);
            return NULL;
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double map_value = *getMapValue(map, j, i);

                if (map_value <= sea_level)
                {
                    sea_map[i * width + j] = sea_level;
                }
                else
                {
                    sea_map[i * width + j] = map_value;
                }
            }
        }
    }

    return sea_map;
}





completeMap* newCompleteMapFromMap(map* map, double sea_level, int display_loading)
{
    completeMap* complete_map = NULL;

    if (map != NULL)
    {
        complete_map = calloc(1, sizeof(completeMap));

        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        complete_map->map = map;
        complete_map->width = width;
        complete_map->height = height;

        complete_map->sea_level = sea_level;

        
        complete_map->sea_values = setSeaLevel(map, sea_level, display_loading);

        complete_map->color_map = generateColorMap(map, sea_level, display_loading);
    }

    return complete_map;
}



completeMap* newCompleteMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                            int size_factors[number_of_layers], double layers_factors[number_of_layers],
                            int map_width, int map_height, double sea_level, int display_loading)
{
    map* map = newMap(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, map_width, map_height, display_loading);

    completeMap* complete_map = NULL;

    if (map != NULL)
    {
        complete_map = newCompleteMapFromMap(map, sea_level, display_loading);
    }

    return complete_map;
}





//? Generate square chunks with automatic size factors.
map* get2dMap(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                    int map_width, int map_height, int display_loading)
{
    int lcm = lcmOfArray(number_of_layers, gradGrids_dimension);

    int size_factors[number_of_layers];

    for (int i = 0; i < number_of_layers; i++)
    {
        size_factors[i] = lcm / gradGrids_dimension[i];
    }

    map* new_map = newMap(number_of_layers, gradGrids_dimension, gradGrids_dimension, size_factors, layers_factors,
                                map_width, map_height, display_loading);

    return new_map;
}



//? Generate square chunks with automatic size factors and creates sea and color maps.
completeMap* fullGen(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                         int map_width, int map_height, double sea_level, int display_loading)
{
    map* new_map = get2dMap(number_of_layers, gradGrids_dimension, layers_factors, map_width, map_height, display_loading);

    completeMap* new_complete_map = newCompleteMapFromMap(new_map, sea_level, display_loading);

    return new_complete_map;
}





void freeCompleteMap(completeMap* completeMap)
{
    if (completeMap != NULL)
    {
        if (completeMap->sea_values != NULL)
        {
            free(completeMap->sea_values);
        }

        if (completeMap->color_map != NULL)
        {
            for (int i = 0; i < completeMap->height; i++)
            {
                for (int j = 0; j < completeMap->width; j++)
                {
                    free(getCompleteMapColor(completeMap, j, i));
                }
            }

            free(completeMap->color_map);
        }


        freeMap(completeMap->map);


        free(completeMap);        
    }
}
