/**
 * @file interpreter.c
 * @author BlueNZ
 * @brief interpreter functions implementation
 * @version 0.1
 * @date 2024-07-27
 * 
 */

#include <bits/types.h>
#include <malloc.h>

#include "map.h"
#include "interpreter.h"


// ------- General functions ------- //

int intpow(int nbr, int exp) {
    if (exp<=0) return 1;
    else if (exp==1) return nbr;
    else if (exp%2==0) return intpow(nbr*nbr,exp/2);
    else return nbr*intpow(nbr,exp-1);
}





// ------- Binary functions ------- //

void freeBytes(bytes b) {
    free(b.bytes);
}

char* hex(unsigned char c, char* res) {
    int a = c/16;
    int b = c%16;
    sprintf(res,"%x%x",a,b);
    return res;
}

void printBytes(bytes bytes, char* start, char* end) {
    char* bytes_str=malloc(bytes.size * 2+4);
    bytes_str[0] = 'b';
    bytes_str[1] = '\'';
    bytes_str[bytes.size * 2+2] = '\'';
    bytes_str[bytes.size * 2+3] = '\0';

    for (int i=0; i<bytes.size; i++) {
        char* c = malloc(3);
        hex(bytes.bytes[i],c);
        bytes_str[2*i+2] = c[0];
        bytes_str[2*i+3] = c[1];
        free(c);
    }
    
    printf("%s",start);
    printf("%s",bytes_str);
    printf("%s",end);
    free(bytes_str);
}

bytes bytesUint8(__uint8_t nbr) {
    bytes res;
    res.bytes = malloc(1);
    res.bytes[0] = nbr;
    res.size=1;
    res.start=0;
    return res;
}

bytes bytesInt(int nbr) {
    bytes res;
    res.bytes = malloc(INT_BITS_NBR/8);
    res.size=INT_BITS_NBR/8;
    res.start=0;

    byte sign = 0;
    if (nbr<0) {
        sign = 128;
        nbr = -nbr;
    }

    res.bytes[0] = sign + (byte) (nbr/(intpow(2,(INT_BITS_NBR-8))));
    for (int i=8; i<INT_BITS_NBR; i+=8) {
        res.bytes[i/8] = (byte) ((nbr % intpow(2,INT_BITS_NBR-i)) / intpow(2, INT_BITS_NBR-i-8));
    }

    return res;
}

bytes bytesDouble(double nbr) {
    bytes res;
    res.bytes = malloc(FLOAT_BITS_NBR/8);
    res.size=FLOAT_BITS_NBR/8;
    res.start=0;

    byte sign = 0;
    if (nbr<0) {
        sign = 128;
        nbr = -nbr;
    }

    int exp = 0;
    if (nbr<intpow(2,FLOAT_BITS_MANTISS)) {
        while (exp>-intpow(2,FLOAT_BITS_EXP-1) && nbr<intpow(2,FLOAT_BITS_MANTISS-1)) {
            exp-=1;
            nbr*=2;
        }
    }
    else {
        while (exp<intpow(2,FLOAT_BITS_EXP-1) && nbr>=intpow(2,FLOAT_BITS_MANTISS)) {
            exp+=1;
            nbr/=2;
        }
    }
    exp+=16;

    res.bytes[0] = sign + (byte) (exp * intpow(2,8-(1+FLOAT_BITS_EXP)) + ((int)nbr)/intpow(2,FLOAT_BITS_MANTISS-(8-(1+FLOAT_BITS_EXP))));
    for (int i=8; i<FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP); i+=8) {
        res.bytes[i/8] = (byte) ((((int)nbr) % intpow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i) / intpow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i-8)));
    }

    return res;
}



tuple_obj_bytes nextUint8(bytes bytes) {
    tuple_obj_bytes res;

    __uint8_t* obj = malloc(1);
    *obj = bytes.bytes[bytes.start];

    res.object = (object) obj;
    res.bytes = bytes;
    bytes.start += 1;

    return res;
}

tuple_obj_bytes nextInt(bytes bytes) {
    tuple_obj_bytes res;

    int* obj = malloc(sizeof(int));
    *obj = 1;

    unsigned int temp = 0;
    for (int i=0; i<INT_BITS_NBR/8; i++) {
        temp*=256;
        temp+=bytes.bytes[bytes.start+i];
    }
    if (temp / intpow(2,INT_BITS_NBR-1)==1) *obj=-(temp%intpow(2,INT_BITS_NBR-1));
    else *obj = temp%intpow(2,INT_BITS_NBR-1);

    res.object = (object) obj;
    res.bytes = bytes;
    bytes.start = bytes.start+INT_BITS_NBR/8;

    return res;
}

tuple_obj_bytes nextDouble(bytes bytes) {
    tuple_obj_bytes res;

    double* obj = malloc(sizeof(double));
    *obj = 0;

    unsigned long temp = 0;
    for (int i=0; i<FLOAT_BITS_NBR/8; i++) {
        temp*=256;
        temp+=bytes.bytes[bytes.start+i];
    }

    int sign = 1;
    if (temp / intpow(2,FLOAT_BITS_NBR-1)==1) {
        temp=temp%FLOAT_BITS_NBR-1;
        sign = -1;
    }

    int exp = temp/intpow(2,FLOAT_BITS_MANTISS);
    exp-=16;
    temp = temp%intpow(2,FLOAT_BITS_MANTISS);
    if (exp>=0) *obj = sign * ((double)temp) * intpow(2,exp);
    else *obj = sign * ((double)temp) / intpow(2,-exp);

    res.object = (object) obj;
    res.bytes = bytes;
    bytes.start = bytes.start+FLOAT_BITS_NBR/8;

    return res;
}