#include <stdio.h>
#include <time.h>

#include "loadingBar.h"

void delay(float waiting_seconds)
{
    clock_t start_time = clock();

    while( (double)(clock() - start_time) / CLOCKS_PER_SEC < waiting_seconds)
    {
        ;
    }
}



void indent_print(int number_of_indents, char str[])
{
    int indent_length = number_of_indents * CUSTOM_INDENT_WIDTH;
    char begin_indent[indent_length + 1];
    
    for (int k = 0; k < number_of_indents; k++)
    {
        for (int l = 0; l < CUSTOM_INDENT_WIDTH; l++)
        {
            int idx = k * CUSTOM_INDENT_WIDTH + l;

            if (l == 0)
            {
                begin_indent[idx] = CUSTOM_POSSESS_CHAR;
                continue;
            }
            begin_indent[idx] = CUSTOM_INDENT_CHAR;
        }
    }
    begin_indent[indent_length] = '\0';

    printf("%s%s", begin_indent, str);
}





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

    printf("%s%s%c%3.2f%%%s", pre_text, loading_bar, INDENT_CHAR, percent, post_text);
}



void predefined_loading_bar(int value, int max_value, int number_of_segments, char* base_str, int number_of_indents, double start_time)
{
    double current_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;

    char end_str[1000] = "";

    char time_str[100] = "";
    sprintf(time_str, " completed - Elapsed time : %.4lf s", current_time);

    if (value == max_value)
    {
        sprintf(end_str, "%s - %sSUCCESS !%s\n", time_str, GREEN_COLOR, DEFAULT_COLOR);
    }
    else
    {
        sprintf(end_str, "%s", time_str);
    }

    char begin_str[1000] = "";
    sprintf(begin_str, "\r%s", base_str);

    int indent_length = number_of_indents * CUSTOM_INDENT_WIDTH;
    char begin_indent[indent_length + 1];
    
    for (int k = 0; k < number_of_indents; k++)
    {
        for (int l = 0; l < CUSTOM_INDENT_WIDTH; l++)
        {
            int idx = k * CUSTOM_INDENT_WIDTH + l;

            if (l == 0)
            {
                begin_indent[idx] = CUSTOM_POSSESS_CHAR;
                continue;
            }
            begin_indent[idx] = CUSTOM_INDENT_CHAR;
        }
    }
    begin_indent[indent_length] = '\0';

    sprintf(begin_str, "\r%s%s", begin_indent, base_str);

    print_loading_bar(value, max_value, NUMBER_OF_SEGMENTS, begin_str, end_str);
}
