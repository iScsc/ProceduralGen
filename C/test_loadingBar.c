#include <stdio.h>
#include <wchar.h> // Used for unicode characters (but does not work yet)
#include <string.h> // For str concatenation

#include "loadingBar.h"

int main()
{
    // printf("Testing unicode characters:\n");

    // wchar_t recantgle = 0x25ad;
    // printf("- This should be a rectangle : ");
    // wprintf(L"%lc\n", recantgle);

    // printf("\nTest : %c", 2588);

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

    // strncat(begin_char, ANSI_COLOR_MAGENTA, 7);

    printf("Voici %s quelque chose qui est un %stest de couleur.%s\n", begin_char, RED_COLOR, DEFAULT_COLOR);

    printf("\nTesting loading bar:\n");

    for (int i = 0; i <= 1000; i++)
    {
        print_loading_bar(i, 1000, 100, begin_char, DEFAULT_COLOR);
        fflush(stdout); // Allows printing without '\n'
        delay(.01);
    }

    printf("\nFinished!\n");

    return 0;
}
