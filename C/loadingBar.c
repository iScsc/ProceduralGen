#include <stdio.h>
#include <time.h>

#include "loadingBar.h"

double pow_int(double value, int power)
{
    if (power == 0)
    {
        return 1.;
    }
    else if (power < 0)
    {
        return 1./pow_int(value, -power);
    }
    else
    {
        if (power % 2 == 0)
        {
            return pow_int(value * value, power/2);
        }
        else
        {
            return value * pow_int(value, power - 1);
        }
    }
}



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

    char format[20] = "";
    snprintf(format, sizeof(format), "%s%d%s", "%s%s%c%3.", PERCENT_DECIMAL, "f%%%s");



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

    printf(format, pre_text, loading_bar, INDENT_CHAR, percent, post_text);
}



void predefined_loading_bar(int value, int max_value, int number_of_segments, char* base_str, int number_of_indents, double start_time)
{
    float previous_percent = ((value - 1) * 100.) / max_value;
    float current_percent = (value * 100.) / max_value;

    // Verifying first if it useful to print it.
    //! Won't work properly if it used on very long loops -> the CPU elapsed time will 'jump' from values to others when a percent changes.

    double precision = pow_int(10., PERCENT_DECIMAL);
    double truncated_prev = ((int) previous_percent * precision)/precision;
    double truncated_curr = ((int) current_percent * precision)/precision;

    if (truncated_prev == truncated_curr)
    {
        return;
    }




    double current_time = (double) (clock() - start_time)/CLOCKS_PER_SEC;

    char end_str[1000] = "";

    char time_str[100] = "";
    snprintf(time_str, sizeof(time_str), " completed - Elapsed CPU time : %.4lf s", current_time);

    if (value == max_value)
    {
        snprintf(end_str, sizeof(end_str), "%s - %sSUCCESS !%s\n", time_str, GREEN_COLOR, DEFAULT_COLOR);
    }
    else
    {
        snprintf(end_str, sizeof(end_str), "%s", time_str);
    }

    char begin_str[1000] = "";
    snprintf(begin_str, sizeof(begin_str), "\r%s", base_str);

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

    snprintf(begin_str, sizeof(begin_str), "\r%s%s", begin_indent, base_str);

    print_loading_bar(value, max_value, NUMBER_OF_SEGMENTS, begin_str, end_str);
}
