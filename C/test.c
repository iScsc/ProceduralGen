#include <stdio.h>
#include <wchar.h> // Used for unicode characters

#include "loadingBar.h"

void main()
{
    // printf("Testing unicode characters:\n");

    // wchar_t recantgle = 0x25ad;
    // printf("- This should be a rectangle : ");
    // wprintf(L"%lc\n", recantgle);

    // printf("\nTest : %c", 2588);


    printf("\nTesting loading bar:\n");

    for (int i = 0; i <= 1000; i++)
    {
        print_loading_bar(i, 1000, 100, "\r", "");
        fflush(stdout); // Allows printing without '\n'
        delay(.01);
    }

    printf("\nFinished!\n");
}
