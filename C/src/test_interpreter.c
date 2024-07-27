/**
 * @file test_interpreter.c
 * @author BlueNZ
 * @brief a testing script for the interpreter functions
 * @version 0.1
 * @date 2024-07-27
 * 
 */

#include <stdio.h>

#include "interpreter.h"

int main() {

    __uint8_t d = 1;
    int a=0,e=-5683;
    double b=1.3,c=-99.9;

    printf("Encoding some numbers : %d, %d, %d, %f, %f\n",d,a,e,b,c);

    byte* db=bytesUint8(d);
    byte* ab=bytesInt(a);
    byte* eb=bytesInt(e);
    byte* bb=bytesDouble(b);
    byte* cb=bytesDouble(c);

    printf("\tUint8 :  %d, %d\n",d,*db);
    printf("\tint :    %d, %d %d %d\n",a,ab[0],ab[1],ab[2]);
    printf("\tint :    %d, %d %d %d\n",e,eb[0],eb[1],eb[2]);
    printf("\tdouble : %f, %d %d %d\n",b,bb[0],bb[1],bb[2]);
    printf("\tdouble : %f, %d %d %d\n",c,cb[0],cb[1],cb[2]);

    d = *((__uint8_t*)nextUint8(0,db)->object);
    a = *((int*)nextInt(0,ab)->object);
    e = *((int*)nextInt(0,eb)->object);
    b = *((double*)nextDouble(0,bb)->object);
    c = *((double*)nextDouble(0,cb)->object);

    printf("Decoding numbers : %d,%d,%d,%f,%f\n",d,a,e,b,c);

}