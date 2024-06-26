/**
 * @file mapGenerator.c
 * @author Zyno and BlueNZ
 * @brief mapGenerator structure and functions implementations
 * @version 0.2
 * @date 2024-06-19
 * 
 */

#include <malloc.h>
#include <time.h>
#include <sys/stat.h>

#include "loadingBar.h"
#include "map.h"
#include "mapGenerator.h"

int gcd(int a, int b)
{
    if (a == 0 || b == 0)
    {
        printf("%sERROR : can't find the greatest common divisor of 0 !%s\n", RED_COLOR, DEFAULT_COLOR);
        return 0;
    }

    if (a < 0 || b < 0)
    {
        printf("%sERROR : this algorithm can't find the greatest common divisor of negative numbers !%s\n", RED_COLOR, DEFAULT_COLOR);
        return -1;
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
    if (a < 0 || b < 0)
    {
        printf("%sERROR : this algorithm can't find the greatest common divisor of negative numbers !%s\n", RED_COLOR, DEFAULT_COLOR);
        return -1;
    }

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

        // We already know that the answer won't change
        if (common == 0)
        {
            return 0;
        }

        else if (common == -1)
        {
            printf("%sERROR : this algorithm can't find the greatest common divisor of negative numbers but array[0] = %d and array[1] = %d !%s\n",
                RED_COLOR, array[0], array[1], DEFAULT_COLOR);
            return -1;
        }


        // Computing the following lcm
        for (int i = 2; i < nb; i++)
        {
            common = lcm(common, array[i]);

            // We already know that the answer won't change
            if (common == 0)
            {
                return 0;
            }

            else if (common == -1)
            {
                printf("%sERROR : this algorithm can't find the greatest common divisor of negative numbers but array[%d] = %d !%s\n",
                 RED_COLOR, i, array[i], DEFAULT_COLOR);
                return -1;
            }
        }
    }

    return common;
}






double* getCompleteMapSeaValue(completeMap* completeMap, int width_idx, int height_idx)
{
    double* sea_value = NULL;

    if (completeMap != NULL)
    {
        int width = completeMap->width;
        int height = completeMap->height;

        if (width_idx < 0 || width_idx >= width)
        {
            printf("%sERROR : invalid width_idx = %d when reading complete map sea value. Should be in range [0, %d]%s\n", RED_COLOR, width_idx, width - 1, DEFAULT_COLOR);
            return NULL;
        }
        
        if (height_idx < 0 || height_idx >= height)
        {
            printf("%sERROR : invalid height_idx = %d when reading complete map sea value. Should be in range [0, %d]%s\n", RED_COLOR, height_idx, height - 1, DEFAULT_COLOR);
            return NULL;
        }

        sea_value = (completeMap->sea_values) + height_idx * width + width_idx;
    }

    return sea_value;
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
    int s = (int) ((value - min_value) * 200. / (sea_level - min_value));

    //? Show red dots on zero values. Could be removed.
    if (value == 0)
    {
        new_color->red_int = 255;
        new_color->green_int = 0;
        new_color->blue_int = 0;

        new_color->red_float = 1.;
        new_color->green_float = 0;
        new_color->blue_float = 0;
    }
    else if (value <= sea_level)
    {
        new_color->red_int = 50;
        new_color->green_int = 50;
        new_color->blue_int = s;

        new_color->red_float = 1./255 * 50;
        new_color->green_float = 1./255 * 50;
        new_color->blue_float = 1./255 * s;
    }
    else
    {
        new_color->red_int = 50;
        new_color->green_int = i;
        new_color->blue_int = 50;

        new_color->red_float = 1./255 * 50;
        new_color->green_float = 1./255 * i;
        new_color->blue_float = 1./255 * 50;
    }

    return new_color;
}





color** generateColorMap(map* map, double sea_level, unsigned int display_loading)
{
    clock_t start_time = clock();

    color** color_map = NULL;

    if (map != NULL)
    {
        // Initialize
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

                if (display_loading != 0)
                {
                    // Begin of the loading bar, which will be finished in the following loop
                    int nb_indents = display_loading - 1;

                    char base_str[100] = "Generating color map...            ";

                    predefined_loading_bar(i * width + j, 2 * height * width - 2, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
                }
            }
        }


        // Generating colors now
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double map_value = *getMapValue(map, j, i);

                color* c = colorize(map_value, sea_level, min_value, max_value);
                
                color_map[i * width + j] = c;

                if (display_loading != 0)
                {
                    // End of the loading bar, which was started in the previous loop
                    int nb_indents = display_loading - 1;

                    char base_str[100] = "Generating color map...            ";

                    predefined_loading_bar(height * width - 1 + i * width + j, 2 * height * width - 2, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
                }
            }
        }
    }

    return color_map;
}



double* setSeaLevel(map* map, double sea_level, unsigned int display_loading)
{
    clock_t start_time = clock();

    double* sea_map = NULL;

    if (map != NULL)
    {
        // Initialize
        int width = map->map_width * map->chunk_width;
        int height = map->map_height * map->chunk_height;

        sea_map = calloc(width * height, sizeof(double));
        if (sea_map == NULL)
        {
            printf("%sSea map allocation was not successful. Returning NULL now...%s\n", RED_COLOR, DEFAULT_COLOR);
            return NULL;
        }

        // Flattening the altitude values on the sea
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


                if (display_loading != 0)
                {
                    int nb_indents = display_loading - 1;

                    char base_str[100] = "Setting sea level...               ";

                    predefined_loading_bar(i * width + j, height * width - 1, NUMBER_OF_SEGMENTS, base_str, nb_indents, start_time);
                }
            }
        }
    }

    return sea_map;
}





completeMap* newCompleteMapFromMap(map* p_map, double sea_level, unsigned int display_loading)
{
    completeMap* complete_map = NULL;

    if (p_map != NULL)
    {
        // Initialize the completeMap structure
        complete_map = calloc(1, sizeof(completeMap));

        int width = p_map->map_width * p_map->chunk_width;
        int height = p_map->map_height * p_map->chunk_height;

        complete_map->map = p_map;
        complete_map->width = width;
        complete_map->height = height;

        complete_map->sea_level = sea_level;

        // Get the corresponding sea map
        complete_map->sea_values = setSeaLevel(p_map, sea_level, display_loading);

        if (display_loading != 0)
        {
            int nb_indents = display_loading - 1;
            indent_print(nb_indents, "\n");
        }
        
        // Get the corresponding color map
        complete_map->color_map = generateColorMap(p_map, sea_level, display_loading);
    }

    return complete_map;
}



completeMap* newCompleteMap(int number_of_layers, int gradGrids_width[number_of_layers], int gradGrids_height[number_of_layers],
                            int size_factors[number_of_layers], double layers_factors[number_of_layers],
                            int map_width, int map_height, double sea_level, unsigned int display_loading)
{
    // Generates a new map from scratch
    map* map = newMap(number_of_layers, gradGrids_width, gradGrids_height, size_factors, layers_factors, map_width, map_height, display_loading);

    completeMap* complete_map = NULL;

    if (map != NULL)
    {
        // If the map generation was successful, generates a completeMap from it
        complete_map = newCompleteMapFromMap(map, sea_level, display_loading);
    }

    return complete_map;
}





map* get2dMap(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                    int map_width, int map_height, unsigned int display_loading)
{
    clock_t start_time = clock();

    int m_loading = display_loading;

    if (display_loading != 0)
    {
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, "Generating a 2d map...\n");

        // The map generation loading bars will be indented once more
        m_loading += 1;
    }

    // Get the final map size
    int lcm = lcmOfArray(number_of_layers, gradGrids_dimension);

    int size_factors[number_of_layers];

    // Generates a new dimension array : size factors should match gradient grids dimensions - 1
    int gradGrid_corresponding_dimensions[number_of_layers];

    for (int i = 0; i < number_of_layers; i++)
    {
        size_factors[i] = lcm / gradGrids_dimension[i];
        gradGrid_corresponding_dimensions[i] = gradGrids_dimension[i] + 1;
    }


    // Generates the corresponding map
    map* new_map = newMap(number_of_layers, gradGrid_corresponding_dimensions, gradGrid_corresponding_dimensions, size_factors, layers_factors,
                                map_width, map_height, m_loading);


    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The 2d map generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_map;
}



completeMap* fullGen(int number_of_layers, int gradGrids_dimension[number_of_layers], double layers_factors[number_of_layers],
                         int map_width, int map_height, double sea_level, unsigned int display_loading)
{
    clock_t start_time = clock();
    int m_loading = display_loading;

    if (display_loading != 0)
    {
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, "Generating a complete map...\n");
        // The map generation loading bars will be indented once more
        m_loading += 1;
    }

    // Generates a new map
    map* new_map = get2dMap(number_of_layers, gradGrids_dimension, layers_factors, map_width, map_height, m_loading);

    if (display_loading != 0)
    {
        int nb_indents = display_loading;
        indent_print(nb_indents, "\n");
    }

    // Generates the completeMap from it
    completeMap* new_complete_map = newCompleteMapFromMap(new_map, sea_level, m_loading);

    if (display_loading == 1)
    {
        double total_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;
        char final_string[200] = "";

        snprintf(final_string, sizeof(final_string), "%sSUCCESS :%s The complete map generation took a total of %.4lf second(s) in CPU time.\n",
                                GREEN_COLOR, DEFAULT_COLOR, total_time);
        
        int nb_indents = display_loading - 1;
        indent_print(nb_indents, final_string);
    }

    return new_complete_map;
}





void writeSeaMapFile(completeMap* completeMap, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Sea Map\n");

        int width = completeMap->width;
        int height = completeMap->height;

        double sea_level = completeMap->sea_level;

        // Writing the parameters
        fprintf(f, "width=%d\nheight=%d\nsea_level=% .8lf\n", width, height, sea_level);

        map* current_map = completeMap->map;
        int map_width = current_map->map_width;
        int map_height = current_map->map_height;

        fprintf(f, "map_width_in_chunks=%d\nmap_height_in_chunks=%d\n", map_width, map_height);

        // Writing the values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double value = *getCompleteMapSeaValue(completeMap, j, i);

                if (j != width - 1)
                {
                    fprintf(f, "% .8lf\t", value);
                }
                else
                {
                    fprintf(f, "% .8lf\n", value);
                }
            }
        }

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



void writeColorIntMapFile(color** color_map, int width, int height, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Color Int Map\n");

        // Writing the parameters
        fprintf(f, "width=%d\nheight=%d\n", width, height);

        // Writing the vectors
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                color* color = color_map[j + i * width];

                if (j != width - 1)
                {
                    fprintf(f, "(%d,%d,%d)\t", color->red_int, color->green_int, color->blue_int);
                }
                else
                {
                    fprintf(f, "(%d,%d,%d)\n", color->red_int, color->green_int, color->blue_int);
                }
            }
        }

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



void writeColorFloatMapFile(color** color_map, int width, int height, char path[])
{
    FILE* f = NULL;

    f = fopen(path, "w");

    if (f != NULL)
    {
        fprintf(f, "Color Float Map\n");

        // Writing the parameters
        fprintf(f, "width=%d\nheight=%d\n", width, height);

        // Writing the vectors
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                color* color = color_map[j + i * width];

                if (j != width - 1)
                {
                    fprintf(f, "(%.4f,%.4f,%.4f)\t", color->red_float, color->green_float, color->blue_float);
                }
                else
                {
                    fprintf(f, "(%.4f,%.4f,%.4f)\n", color->red_float, color->green_float, color->blue_float);
                }
            }
        }

        fclose(f);
    }
    else
    {
        printf("%sERROR : could not open file in writing mode at path '%s'%s\n", RED_COLOR, path, DEFAULT_COLOR);
        return;
    }
}



void writeCompleteMapFiles(completeMap* complete_map, char folder_path[])
{
    //! TEMPORARY? -> File storage may be ineffective because of the space complexity of it.
    //! These files are big!
    struct stat st = {0};

    // Generates a directory if it does not exist
    if (stat(folder_path, &st) == -1)
    {
        // Should check what 0700 permissions are exactly, but that's how it works to create a directory.
        mkdir(folder_path, 0700);
    }

    // Generates the sea map file
    char sea_map_path[200] = "";
    snprintf(sea_map_path, sizeof(sea_map_path), "%ssea_map.txt", folder_path);
    writeSeaMapFile(complete_map, sea_map_path);


    // Generates the color int map file
    char color_int_path[200] = "";
    snprintf(color_int_path, sizeof(color_int_path), "%scolor_int_map.txt", folder_path);
    writeColorIntMapFile(complete_map->color_map, complete_map->width, complete_map->height, color_int_path);


    // Generates the color float map file
    char color_float_path[200] = "";
    snprintf(color_float_path, sizeof(color_float_path), "%scolor_float_map.txt", folder_path);
    writeColorFloatMapFile(complete_map->color_map, complete_map->width, complete_map->height, color_float_path);
}



completeMap* readCompleteMapFiles(char path[]);





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
