/**
 * @file test_interpreter.c
 * @author BlueNZ
 * @brief a testing script for the interpreter functions
 * @version 0.1
 * @date 2024-07-27
 * 
 */

#include <stdio.h>
#include <malloc.h>

#include "interpreter.h"

int main() {

    __uint8_t d = 1;
    int a=0,e=-5683;
    double b=1.3,c=-99.9;

    printf("Encoding some numbers : %d, %d, %d, %f, %f\n",d,a,e,b,c);

    bytes db=bytesUint8(d);
    bytes ab=bytesInt(a);
    bytes eb=bytesInt(e);
    bytes bb=bytesDouble(b);
    bytes cb=bytesDouble(c);

    printf("\tUint8 :  %d, ",d);
    printBytes(db,"","\n");
    printf("\tint :    %d, ",a);
    printBytes(ab,"","\n");
    printf("\tint :    %d, ",e);
    printBytes(eb,"","\n");
    printf("\tdouble : %lf, ",b);
    printBytes(bb,"","\n");
    printf("\tdouble : %lf, ",c);
    printBytes(cb,"","\n");

    object od = nextUint8(db).object;
    object oa = nextInt(ab).object;
    object oe = nextInt(eb).object;
    object ob = nextDouble(bb).object;
    object oc = nextDouble(cb).object;

    d = *((__uint8_t*)od);
    a = *((int*)oa);
    e = *((int*)oe);
    b = *((double*)ob);
    c = *((double*)oc);

    free(od);
    free(oa);
    free(oe);
    free(ob);
    free(oc);

    freeBytes(db);
    freeBytes(ab);
    freeBytes(eb);
    freeBytes(bb);
    freeBytes(cb);

    printf("Decoding numbers : %d, %d, %d, %f, %f\n",d,a,e,b,c);

    return 0;
}