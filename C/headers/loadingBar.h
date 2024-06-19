/**
 * @file loadingBar.h
 * @author Zyno
 * @brief Header to the loading bar functions
 * @version 0.1
 * @date 2024-05-21
 * 
 */

#ifndef LOADING_BAR
#define LOADING_BAR

// ##### Definitions ####################

// ----- Colors

#define DEFAULT_COLOR       "\033[0m"   /**< the string code of the default terminal color*/

#define GREEN_COLOR         "\033[92m"  /**< the string code of the green terminal color*/
#define RED_COLOR           "\033[91m"  /**< the string code of the red terminal color*/
#define YELLOW_COLOR        "\033[93m"  /**< the string code of the yellow terminal color*/
#define BLUE_COLOR          "\033[94m"  /**< the string code of the blue terminal color*/
#define PURPLE_COLOR        "\033[95m"  /**< the string code of the purple terminal color*/

#define ANSI_COLOR_RED      "\x1b[31m"  /**< the string code of the ANSI terminal red color*/
#define ANSI_COLOR_GREEN    "\x1b[32m"  /**< the string code of the ANSI terminal green color*/
#define ANSI_COLOR_YELLOW   "\x1b[33m"  /**< the string code of the ANSI terminal yellow color*/
#define ANSI_COLOR_BLUE     "\x1b[34m"  /**< the string code of the ANSI terminal blue color*/
#define ANSI_COLOR_MAGENTA  "\x1b[35m"  /**< the string code of the ANSI terminal magenta color*/
#define ANSI_COLOR_CYAN     "\x1b[36m"  /**< the string code of the ANSI terminal cyan color*/
#define ANSI_COLOR_RESET    "\x1b[0m"   /**< the string code of the ANSI terminal color reset*/

// ----- Characters
#define HORIZONTAL_RECTANGLE_CHAR '-'   /**< the horizontal rectangle character code - which is `"\\u25ac"` in unicode (with a single \ obviously)*/

// ----- Constants
#define NUMBER_OF_SEGMENTS  21          /**< a recommended constant value for the number of segments in the loading bars*/
#define PERCENT_DECIMAL     1           /**< a constant value used for the percentage precision when printing the loading bars : `94.2%` if set to 1 for instance*/

#define INDENT_CHAR  '\t'               /**< a constant value for the indent character*/

#define CUSTOM_INDENT_CHAR  ' '         /**< the character used for custom indents blanks*/
#define CUSTOM_POSSESS_CHAR '|'         /**< the character used to print possession trees*/
#define CUSTOM_INDENT_WIDTH  4          /**< the width of the custom indents*/

// ##### End - Definitions ##############

/**
 * @brief Waits the given delay before returning.
 * 
 * @param seconds (float) : the amount of time to delay in seconds.
 * 
 * @note The delayed time will be impacted by the use of your processor.
 * The given `seconds` input  will be be the CPU waiting time, not the actual time.
 */
void delay(float seconds);

/**
 * @brief Prints the given char with the given number of indents. Made to build possess trees.
 * 
 * @param number_of_indents (int) : the number of indents printed before the actual string.
 * @param str (char[]) : the string to print after the indent.
 * 
 * @note This does not support '\\n' characters in the given str.
 */
void indent_print(int number_of_indents, char str[]);



/**
 * @brief Prints a loading bar line, with the given parameters.
 * Made to monitor loops.
 * 
 * @param value (int) : the current value of the loop.
 * @param max_value (int) : the maximum value reachable.
 * @param number_of_segments (int) : the number of segments in the loading bar.
 * @param pre_text (char[]) : the text to be printed before the loading bar.
 * @param post_text (char[]) : the text to be printed after the loading bar.
 * 
 * @note No '\\r' are required in the pre_text or post_text. It is automatically added.
 */
void print_loading_bar(int value, int max_value, int number_of_segments, char pre_text[], char post_text[]);

/**
 * @brief Prints a predefined loading bar with the given parameters.
 * Made to monitor loops in a predefined stylish way, without useless prints.
 * 
 * @param value (int) : the current value of the loop.
 * @param max_value (int) : the maximum value reachable.
 * @param number_of_segments (int) : the number of segments in the loading bar.
 * @param base_str (char[]) : the base text to print before the loading bar.
 * @param number_of_indents (int) : number of indents to use in the loading bar printing.
 * @param start_time (double) : starting time of the loop. Passed to print the elapsed CPU time since the beginning of the loop.
 * 
 * @note `base_text` should not contain the '\\r' character.
 */
void predefined_loading_bar(int value, int max_value, int number_of_segments, char base_str[], int number_of_indents, double start_time);

#endif
