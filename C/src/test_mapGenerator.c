/**
 * @file test_mapGenerator.c
 * @author Zyno
 * @brief a testing script for the whole mapGenerator implementations
 * @version 0.1
 * @date 2024-05-21
 * 
 */

#include <stdio.h>
#include <time.h>

#include "mapGenerator.h"

int main()
{
    //? Booleans to decide which tests to do
    int gcd_testing = 0;
    int lcm_testing = 0;
    int lcm_array_testing = 0;

    int complete_map_generation_testing = 1;


    // ------------- Testing ----------

    // gcd testing
    if (gcd_testing == 1)
    {
        int a = 31;
        int b = 16;

        int c = gcd(a, b);

        printf("The Greatest Common Divisor of a = %d and b = %d is gcd = %d\n", a, b, c);
    }



    // lcm testing
    if (lcm_testing == 1)
    {
        int d = 3;
        int e = 8;

        int f = lcm(d, e);

        printf("The Lowest Common Multiple of d = %d and e = %d is lcm = %d\n", d, e, f);
    }



    // lcm of an array testing
    if (lcm_array_testing == 1)
    {
        int nb = 4;
        int array[] = {5, 41, 7, 2};

        int array_lcm = lcmOfArray(nb, array);

        printf("The Lowest Common Multiple of the array {");
        for (int n = 0; n < nb; n++)
        {
            if (n < nb - 1)
            {
                printf("%d, ", array[n]);
            }
            else
            {
                printf("%d}", array[n]);
            }
        }
        printf(" is %d.\n", array_lcm);
    }



    // fullGen testing
    if (complete_map_generation_testing == 1)
    {
        clock_t start_time = clock();

        time_t seed = time(NULL);
        // long int seed = 1714995019;    //436517554376;       //1715794433;

        printf("Seed used for the generation : %ld\n", seed);

        setRandomSeed(seed); //? Comment this to make it not random, or give a constant rather than time(NULL)

        int nb_layers = 3;

        //! Remember that the memory space is square of the dimension. Be cautious!
        int dimensions[] = {2, 5, 11};
        int final_size = lcmOfArray(nb_layers, dimensions);
        printf("Final size per chunk : %d\n", final_size);

        double weights[] = {1, .1, .01};

        int width = 4;   //7;
        int height = 5;   //5;

        double sea_level = -.15;

        int display_loading = 1;      //1;



        //? MEMORY SPACE REQUIRED
        long int mem_space_by_chunk = sizeof(chunk) + final_size * final_size * sizeof(double) + nb_layers * (sizeof(double) + sizeof(layer*))
                                        + sizeof(layer) + final_size * final_size * sizeof(double);
        
        for (int i = 0; i < nb_layers; i++)
        {
            mem_space_by_chunk += sizeof(gradientGrid) + dimensions[i] * dimensions[i] * sizeof(vector);
        }

        long int total_memory_space_used = sizeof(completeMap) + width * final_size * height * final_size * (sizeof(double) + sizeof(color*) + sizeof(color)) 
                                            + sizeof(map) + width * final_size * height * final_size * sizeof(double)
                                            + width * height * (sizeof(chunk*) + mem_space_by_chunk);
        
        printf("The complete memory space used will be around %ld bytes.\n", total_memory_space_used);
        printf("Do you want to continue ? (1 for Yes, 0 for No)   ->   ");

        int user_input = -1;
        scanf("%d", &user_input);

        int request = 1;
        int max_requests = 3;

        while (user_input != 1 && user_input != 0 && request < max_requests)
        {
            printf("Your input was not valid. Try again. (You have %d remaining attempts)\n\n", max_requests - request);
            request += 1;

            printf("The complete memory space used will be around %ld bytes.\n", total_memory_space_used);
            printf("Do you want to continue ? (1 for Yes, 0 for No)   ->   ");

            scanf("%d", &user_input);
        }
        printf("\n");

        if (user_input != 1)
        {
            printf("The process was cancelled.\n");
            return 1;
        }



        printf("Generating a complete map with given parameters...\n");
        completeMap* new_complete_map = fullGen(nb_layers, dimensions, weights, width, height, sea_level, display_loading);
        printf("Generation complete!\n");





        //? Comment this if you don't want to save it in a file.
        //! WARNING : ../saves/ the folder must exist for it to work properly
        // printf("File creation...\n");
        // char folder_path[200] = "../saves/completeMap_test/";

        // writeCompleteMapFiles(new_complete_map, folder_path);
        // printf("File should be written now.\n");



        printf("Deallocating now...\n");
        freeCompleteMap(new_complete_map);

        double total_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
        printf("The whole map generation of lcm %d size %d x %d took a total of %lf second(s) in CPU time\n", final_size, width, height, total_time);
    }

    return 0;
}
