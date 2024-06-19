/**
 * @file test_unicode.c
 * @author Zyno
 * @brief a file to test some unicode characters printing in order to improve loading bars.
 * @version 0.1
 * @date 2024-06-19
 * 
 */

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main() {
    setlocale(LC_CTYPE, "");
    wchar_t star = 0x2605;
    wchar_t integral = 0x222b;
    wprintf(L"%lc\n", star);
    wprintf(L"%lc\n", integral);

    wchar_t rectangle = 0x25ac;
    
    wprintf(L"[");
    for (int i = 0; i < 100; i ++)
    {
        wprintf(L"%lc", rectangle);
    }
    wprintf(L"]\n");

    wprintf(L"\n\n");

    wchar_t square = 0x25a0;

    wprintf(L"[");
    for (int i = 0; i < 100; i ++)
    {
        wprintf(L"%lc", square);
    }
    wprintf(L"]\n");
}