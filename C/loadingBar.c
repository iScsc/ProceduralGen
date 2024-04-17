#include <stdio.h>
#include <time.h>

#include "loadingBar.h"

void print_loading_bar(int value, int max_value, int number_of_segments, char* pre_text, char* post_text)
{
    char begin_char = '[';
    char end_char = ']';
    char loading_char = HORIZONTAL_RECTANGLE_CHAR;



    float value_per_segment = (float) max_value / number_of_segments;
    int number_filled = value / value_per_segment;

    char loading_bar[number_of_segments + 3];
    loading_bar[0] = begin_char;


    if (value == max_value)
    {
        number_filled = number_of_segments;
    }

    for (int i = 0; i < number_of_segments; i++)
    {
        if (i < number_filled)
        {
            loading_bar[1 + i] = loading_char;
        }
        else
        {
            loading_bar[1 + i] = ' ';
        }
    }

    loading_bar[1 + number_of_segments] = end_char;
    loading_bar[2 + number_of_segments] = '\0';


    float percent = (value * 100.) / max_value;

    printf("%s%s     %3.2f%%%s", pre_text, loading_bar, percent, post_text);
}





void delay(float waiting_seconds)
{
    clock_t start_time = clock();

    while( (double)(clock() - start_time) / CLOCKS_PER_SEC < waiting_seconds)
    {
        ;
    }
}
