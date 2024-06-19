/**
 * @file test_loadingBar.c
 * @author Zyno
 * @brief a testing script for the loading_bar implementation
 * @version 0.1
 * @date 2024-05-21
 * 
 */

#include <stdio.h>
#include <time.h>

#include "loadingBar.h"

int main()
{
    char begin_char[1 + 7 + 1] = "\r";
    
    // get end of the begin_char
    int k = 0;
    while (begin_char[k] != '\0')
    {
        k+=1;
    }

    char concatenate[7] = PURPLE_COLOR;

    int j = 0;
    while (concatenate[j] != '\0')
    {
        begin_char[k + j] = concatenate[j];
        j++;
    }
    begin_char[k + j] = '\0';

    printf("Voici %s quelque chose qui est un %stest de couleur.%s\n", begin_char, RED_COLOR, DEFAULT_COLOR);

    printf("\nTesting loading bar:\n");

    for (int i = 0; i <= 500; i++)
    {
        print_loading_bar(i, 500, 100, begin_char, DEFAULT_COLOR);
        delay(.01);
    }

    printf("\nFinished!\n");

    int indent_level = 2;

    indent_print(indent_level - 1, "Testing the predefined loading bar and the indent print:\n");

    char base[100] = "This is a testing loading bar ";
    double start_time = (double) clock();

    for (int i = 0; i <= 500; i++)
    {
        predefined_loading_bar(i, 500, 50, base, indent_level, start_time);
        delay(.01);
    }

    return 0;
}
