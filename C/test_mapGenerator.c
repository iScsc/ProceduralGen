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
        setRandomSeed(time(NULL)); //? Comment this to make it not random, or give a constant rather than time(NULL)

        int nb_layers = 2;

        int dimensions[] = {3, 7};

        double weights[] = {1, .1};

        int width = 3;
        int height = 2;

        double sea_level = -.1;

        int display_loading = 1;

        printf("Generating a complete map with given parameters...\n");
        completeMap* new_complete_map = fullGen(nb_layers, dimensions, weights, width, height, sea_level, display_loading);
        printf("Generation complete!\n");

        printf("Deallocating now...\n");
        freeCompleteMap(new_complete_map);
    }

    return 0;
}
