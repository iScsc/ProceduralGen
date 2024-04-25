#ifndef LOADING_BAR
#define LOADING_BAR

// ##### Definitions ####################

// ----- Colors
#define DEFAULT_COLOR "\033[0m"

#define GREEN_COLOR "\033[92m"
#define RED_COLOR "\033[91m"
#define YELLOW_COLOR "\033[93m"
#define BLUE_COLOR "\033[94m"
#define PURPLE_COLOR "\033[95m"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ----- Characters
#define HORIZONTAL_RECTANGLE_CHAR '-'   //"\u25ac"

// ##### End - Definitions ##############

void print_loading_bar(int value, int max_value, int number_of_segments, char* pre_text, char* post_text);

void delay(float seconds);

#endif