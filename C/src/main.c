/**
 * @file main.c
 * @author Zyno
 * @brief main code to generate a new complete map
 * @version 0.1
 * @date 2024-07-04
 * 
 */

#include <stdio.h>
#include <time.h>

#include "loadingBar.h"
#include "mapGenerator.h"


#define INPUT_MAX_SIZE 100



int main(int argc, char* argv[argc])
{
    //? --- Initialisation of the different variables -----------------------------------------------------------------------------------------------

    // Can be set in command line
    int map_width = 0;
    int map_height = 0;
    int nb_layers = 0;
    
    double sea_level = 0.;
    int sea_level_affected = 0; // boolean

    //? --- Manage command line inputs : ./main <map_width> <map_height> <nb_layers> <sea_level> ----------------------------------------------------

    if (argc > 1)
    {
        int temp = 0;
        int nb_associated = sscanf(argv[1], "%d", &temp);

        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : input '%s' could not be cast as an integer for map_width /!\\%s\n", RED_COLOR, argv[1], DEFAULT_COLOR);
        }
        else if (temp <= 0)
        {
            printf("%s/!\\ Value Error : map_width should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
        }
        else
        {
            map_width = temp;
        }
    }
    if (argc > 2)
    {
        int temp = 0;
        int nb_associated = sscanf(argv[2], "%d", &temp);

        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : input '%s' could not be cast as an integer for map_height /!\\%s\n", RED_COLOR, argv[2], DEFAULT_COLOR);
        }
        else if (temp <= 0)
        {
            printf("%s/!\\ Value Error : map_height should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
        }
        else
        {
            map_height = temp;
        }
    }
    if (argc > 3)
    {
        int temp = 0;
        int nb_associated = sscanf(argv[3], "%d", &temp);

        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : input '%s' could not be cast as an integer for nb_layers /!\\%s\n", RED_COLOR, argv[3], DEFAULT_COLOR);
        }
        else if (temp <= 0)
        {
            printf("%s/!\\ Value Error : nb_layers should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
        }
        else
        {
            nb_layers = temp;
        }
    }
    if (argc > 4)
    {
        double temp = 0.;
        int nb_associated = sscanf(argv[4], "%lf", &temp);

        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : input '%s' could not be cast as a double for sea_level /!\\%s\n", RED_COLOR, argv[4], DEFAULT_COLOR);
        }
        else
        {
            sea_level = temp;
            sea_level_affected = 1;
        }
    }
    if (argc > 5)
    {
        printf("%s/!\\ Warning : the remaining inputs will not be treated : /!\\%s\n", YELLOW_COLOR, DEFAULT_COLOR);
        for (int i = 5; i < argc; i++)
        {
            if (i < argc - 1)
            {
                printf("'%s', ", argv[i]);
            }
            else
            {
                printf("'%s'\n", argv[i]);
            }
        }
    }





    //? --- Manage missing values with prompt inputs (will have to manage layers whatsoever) --------------------------------------------------------

    if ( !(map_width > 0) )
    {
        int temp = 0;
        int nb_associated = 0;
        char line[INPUT_MAX_SIZE] = "";

        while ( !(temp > 0) )
        {
            printf("Please enter a valid map_width value for the map generation. It should be > 0. Your input : ");
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %d", &temp);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
            else if (temp <= 0)
            {
                printf("%s/!\\ Value Error : map_width should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
            }
        }

        map_width = temp;
    }
    printf("%sMap width is set to %d%s\n\n", GREEN_COLOR, map_width, DEFAULT_COLOR);


    if ( !(map_height > 0) )
    {
        int temp = 0;
        int nb_associated = 0;
        char line[INPUT_MAX_SIZE] = "";

        while ( !(temp > 0) )
        {
            printf("Please enter a valid map_height value for the map generation. It should be > 0. Your input : ");
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %d", &temp);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
            else if (temp <= 0)
            {
                printf("%s/!\\ Value Error : map_height should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
            }
        }

        map_height = temp;
    }
    printf("%sMap height is set to %d%s\n\n", GREEN_COLOR, map_height, DEFAULT_COLOR);


    if ( !(nb_layers > 0) )
    {
        int temp = 0;
        int nb_associated = 0;
        char line[INPUT_MAX_SIZE] = "";

        while ( !(temp > 0) )
        {
            printf("Please enter a valid nb_layers value for the map generation. It should be > 0. Your input : ");
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %d", &temp);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
            else if (temp <= 0)
            {
                printf("%s/!\\ Value Error : nb_layers should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp, DEFAULT_COLOR);
            }
        }

        nb_layers = temp;
    }
    printf("%sNumber of layers is set to %d%s\n\n", GREEN_COLOR, nb_layers, DEFAULT_COLOR);




    //? --- Manage layers parameters ----------------------------------------------------------------------------------------------------------------


    // Initialisation of the missing variables

    int grid_dimensions[nb_layers];
    double layers_factors[nb_layers];

    for (int i=0; i < nb_layers; i++)
    {
        printf("Setting up the layer n°%d/%d...\n", i+1, nb_layers);

        int nb_associated = 0;
        int temp_i = 0;
        double temp_d = 0.;
        char line[INPUT_MAX_SIZE] = "";

        while ( !(temp_i > 0) )
        {
            printf("Please enter a valid grid dimension value for gradientGrid %d/%d. It should be > 0. Your input : ", i+1, nb_layers);
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %d", &temp_i);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
            else if (temp_i <= 0)
            {
                printf("%s/!\\ Value Error : a grid dimension should be a strictly positive integer but %d was given /!\\%s\n", RED_COLOR, temp_i, DEFAULT_COLOR);
            }
        }

        grid_dimensions[i] = temp_i;
        printf("Grid dimensions %d/%d set to %d\n", i+1, nb_layers, grid_dimensions[i]);




        // Reset tracking parameters
        nb_associated = 0;
        snprintf(line, sizeof(line), "");

        while ( (nb_associated == 0) )
        {
            printf("Please enter a valid layer factor value for layer %d/%d. It should be a double value. Your input : ", i+1, nb_layers);
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %lf", &temp_d);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as a double value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
        }

        layers_factors[i] = temp_d;
        printf("Layer factor %d/%d set to %lf\n\n", i+1, nb_layers, layers_factors[i]);
    }



    // Summary of the values set

    printf("Gradient grid sizes (-1) are finally set to the following values :\n");
    for (int i = 0; i < nb_layers; i++)
    {
        if (i == 0)
        {
            printf("%s{%d, ", GREEN_COLOR, grid_dimensions[i]);
        }
        else if (i < nb_layers - 1)
        {
            printf("%d, ", grid_dimensions[i]);
        }
        else
        {
            printf("%d}%s\n", grid_dimensions[i], DEFAULT_COLOR);
        }
    }
    int final_size = lcmOfArray(nb_layers, grid_dimensions);
    printf("It will lead to square chunks of sizes %s%d%s\n\n", GREEN_COLOR, final_size, DEFAULT_COLOR);




    printf("Layers factors are finally set to the following values :\n");
    for (int i = 0; i < nb_layers; i++)
    {
        if (i == 0)
        {
            printf("%s{%lf, ", GREEN_COLOR, layers_factors[i]);
        }
        else if (i < nb_layers - 1)
        {
            printf("%lf, ", layers_factors[i]);
        }
        else
        {
            printf("%lf}%s\n\n", layers_factors[i], DEFAULT_COLOR);
        }
    }



    //? --- Manage final missing parameter : sea_level ----------------------------------------------------------------------------------------------

    if ( (sea_level_affected != 1) )
    {
        int nb_associated = 0;
        char line[INPUT_MAX_SIZE] = "";

        while ( sea_level_affected != 1 )
        {
            printf("Please enter a valid sea_level value for the map generation. It should be a double value. Your input : ");
            
            fgets(line, INPUT_MAX_SIZE, stdin);

            nb_associated = sscanf(line, " %lf", &sea_level);
            if (nb_associated == 0)
            {
                printf("%s/!\\ Input Error : could not convert line '%s' as a double value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
            }
            else
            {
                sea_level_affected = 1;
            }
        }
    }
    printf("%sSea level is set to %lf%s\n\n", GREEN_COLOR, sea_level, DEFAULT_COLOR);





    //? --- Generation ------------------------------------------------------------------------------------------------------------------------------

    // Required memory space
    long int mem_space_by_chunk = sizeof(chunk) + final_size * final_size * sizeof(double) + nb_layers * (sizeof(double) + sizeof(layer*))
                                    + sizeof(layer) + final_size * final_size * sizeof(double);
    
    for (int i = 0; i < nb_layers; i++)
    {
        mem_space_by_chunk += sizeof(gradientGrid) + grid_dimensions[i] * grid_dimensions[i] * sizeof(vector);
    }

    long int total_memory_space_used = sizeof(completeMap) + map_width * final_size * map_height * final_size * (sizeof(double) + sizeof(color*) + sizeof(color)) 
                                        + sizeof(map) + map_width * final_size * map_height * final_size * sizeof(double)
                                        + map_width * map_height * (sizeof(chunk*) + mem_space_by_chunk);
    
    printf("The complete memory space used will be around %s%ld bytes%s.\n", BLUE_COLOR, total_memory_space_used, DEFAULT_COLOR);


    int user_input = -1;
    int temp = -1;
    int nb_associated = 0;
    char line[INPUT_MAX_SIZE] = "";

    while ( temp != 0 && temp != 1 )
    {
        printf("Do you want to continue ? (1 for Yes, 0 for No)   ->   ");
        
        fgets(line, INPUT_MAX_SIZE, stdin);

        nb_associated = sscanf(line, " %d", &temp);
        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
        }
        else if (temp != 0 && temp != 1)
        {
            printf("%s/!\\ Value Error : please enter 0 to cancel or 1 to proceed with the generation. /!\\%s\n", RED_COLOR, DEFAULT_COLOR);
        }
    }

    user_input = temp;
    printf("\n");

    if (user_input != 1)
    {
        printf("The process was cancelled.\n");
        return 0; // The process was correctly cancelled.
    }



    // Whether to display loading bars or not

    int display_loading = -1; // boolean

    // Reset tracking parameters
    temp = -1;
    nb_associated = 0;
    snprintf(line, sizeof(line), "");

    while ( temp != 0 && temp != 1 )
    {
        printf("Do you want to print loading bars during the generation ? (1 for Yes, 0 for No)   ->   ");
        
        fgets(line, INPUT_MAX_SIZE, stdin);

        nb_associated = sscanf(line, " %d", &temp);
        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
        }
        else if (temp != 0 && temp != 1)
        {
            printf("%s/!\\ Value Error : please enter 0 to hide loading bars or 1 to show them. /!\\%s\n", RED_COLOR, DEFAULT_COLOR);
        }
    }

    display_loading = temp;


    // TODO : Ask for a seed input


    printf("Generating a complete map with given parameters...\n");
    clock_t start_time = clock();


    completeMap* new_complete_map = fullGen(nb_layers, grid_dimensions, layers_factors, map_width, map_height, sea_level, display_loading);
    printf("Generation complete!\n");

    double total_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
    printf("The whole map generation took a total of %lf second(s) in CPU time\n", total_time);


    // TODO : Ask for a path to save the map// Whether to display loading bars or not

    int save_map = -1; // boolean

    // Reset tracking parameters
    temp = -1;
    nb_associated = 0;
    snprintf(line, sizeof(line), "");

    while ( temp != 0 && temp != 1 )
    {
        printf("Do you want to save the generated map ? (1 for Yes, 0 for No)   ->   ");
        
        fgets(line, INPUT_MAX_SIZE, stdin);

        nb_associated = sscanf(line, " %d", &temp);
        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : could not convert line '%s' as an integer value. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
        }
        else if (temp != 0 && temp != 1)
        {
            printf("%s/!\\ Value Error : please enter 0 to hide loading bars or 1 to show them. /!\\%s\n", RED_COLOR, DEFAULT_COLOR);
        }
    }

    save_map = temp;


    if (save_map==1) {
        printf("Save file creation : \n");
        start_time = clock();
        
        // Get folder name
        temp = -1;
        nb_associated = 0;
        snprintf(line, sizeof(line), "");

        char folder_path[100] = "../saves";

        printf("Folder name (defaults to %s)   ->   ", folder_path);
        fgets(line, INPUT_MAX_SIZE, stdin);
        nb_associated = sscanf(line, " %s", &folder_path);
        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : could not convert line '%s' as a path name. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
        }
        
        // Get file name
        temp = -1;
        nb_associated = 0;
        snprintf(line, sizeof(line), "");

        char name[100] = "save.data";

        printf("File name (defaults to %s)   ->   ", name);
        fgets(line, INPUT_MAX_SIZE, stdin);

        nb_associated = sscanf(line, " %s", &name);
        if (nb_associated == 0)
        {
            printf("%s/!\\ Input Error : could not convert line '%s' as a file name. /!\\%s\n", RED_COLOR, line, DEFAULT_COLOR);
        }

        // Get bytes
        bytes b;
        bytes cb = bytesCompleteMap(new_complete_map);
        b.size = cb.size +1;
        b.start = 0;
        b.bytes = malloc(b.size * sizeof(byte));
        b.bytes[0] = BYTES_VERSION;
        concatBytes(b,cb,1);

        // Write binary file
        writeBytesFile(b,folder_path,name);

        // Deallocating
        freeBytes(b);
        freeBytes(cb);

        total_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
        printf("The map saving took a total of %lf second(s) in CPU time\n", total_time);
    }



    printf("Deallocating now...\n");
    freeCompleteMap(new_complete_map);



    return 0;
}