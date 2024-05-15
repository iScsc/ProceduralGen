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
        // long int seed = 1714995019;    //436517554376;

        printf("Seed used for the generation : %ld\n", seed);

        setRandomSeed(seed); //? Comment this to make it not random, or give a constant rather than time(NULL)

        int nb_layers = 3;

        //! Remember that the memory space is square of the dimension. Be cautious!
        //TODO : Make something to compute the required memory space and ask for user if he really wants to do what he asked.
        int dimensions[] = {2, 5, 11};
        int final_size = lcmOfArray(nb_layers, dimensions);
        printf("Final size : %d\n", final_size);

        double weights[] = {1, .1, .01};

        int width = 7;   //7;
        int height = 5;   //5;

        double sea_level = -.15;

        int display_loading = 1;      //1;

        printf("Generating a complete map with given parameters...\n");
        completeMap* new_complete_map = fullGen(nb_layers, dimensions, weights, width, height, sea_level, display_loading);
        printf("Generation complete!\n");




        printf("File creation...\n");
        char folder_path[200] = "../saves/completeMap_test/";

        writeCompleteMapFiles(new_complete_map, folder_path);
        printf("File should be written now.\n");



        printf("Deallocating now...\n");
        freeCompleteMap(new_complete_map);

        double total_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
        printf("The whole map generation of lcm %d took a total of %lf second(s) in CPU time\n", final_size, total_time);
    }

    return 0;
}
