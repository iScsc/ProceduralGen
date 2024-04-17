#ifndef LOADING_BAR
#define LOADING_BAR

// ##### Definitions ####################

// ----- Colors
#define GREEN_COLOR "\033[92m" //"\e[0;32m" //'\033[92m'
#define DEFAULT_COLOR "\033[0m"

// ----- Characters
#define HORIZONTAL_RECTANGLE_CHAR '-'   //"\u25ac"

// ##### End - Definitions ##############

void print_loading_bar(int value, int max_value, int number_of_segments, char* pre_text, char* post_text);

void delay(float seconds);

#endif
