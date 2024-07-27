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

byte* bytesUint8(__uint8_t nbr) {
    byte* res = malloc(1);
    *res = nbr;
    return res;
}

byte* bytesInt(int nbr) {
    byte* res = malloc(INT_BITS_NBR/8);

    byte sign = 0;
    if (nbr<0) {
        sign = 128;
        nbr = -nbr;
    }

    res[0] = sign + (byte) (nbr/(intpow(2,(INT_BITS_NBR-8))));
    for (int i=8; i<INT_BITS_NBR; i+=8) {
        res[i/8] = (byte) ((nbr % intpow(2,INT_BITS_NBR-i)) / intpow(2, INT_BITS_NBR-i-8));
    }

    return res;
}

byte* bytesDouble(double nbr) {
    byte* res = malloc(INT_BITS_NBR/8);

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

    res[0] = sign + (byte) (exp * intpow(2,8-(1+FLOAT_BITS_EXP)) + ((int)nbr)/intpow(2,FLOAT_BITS_MANTISS-(8-(1+FLOAT_BITS_EXP))));
    for (int i=8; i<FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP); i+=8) {
        res[i/8] = (byte) ((((int)nbr) % intpow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i) / intpow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i-8)));
    }

    return res;
}



tuple_obj_bytes* nextUint8(int start, byte* bytes) {
    tuple_obj_bytes* res = malloc(sizeof(tuple_obj_bytes));

    __uint8_t* obj = malloc(1);
    *obj = bytes[start];

    res->object = (object) obj;
    res->bytes = bytes;
    res->start = start+1;

    return res;
}

tuple_obj_bytes* nextInt(int start, byte* bytes) {
    tuple_obj_bytes* res = malloc(sizeof(tuple_obj_bytes));

    int* obj = malloc(sizeof(int));
    *obj = 0;
    for (int i=0; i<INT_BITS_NBR/8; i++) {
        *obj*=256;
        *obj+=bytes[start+i];
    }
    if (*obj / intpow(2,INT_BITS_NBR-1)==1) *obj=-(*obj%INT_BITS_NBR-1);

    res->object = (object) obj;
    res->bytes = bytes;
    res->start = start+INT_BITS_NBR/8;

    return res;
}

tuple_obj_bytes* nextDouble(int start, byte* bytes) {
    tuple_obj_bytes* res = malloc(sizeof(tuple_obj_bytes));

    double* obj = malloc(sizeof(double));
    *obj = 0;
    for (int i=0; i<FLOAT_BITS_NBR/8; i++) {
        *obj*=256;
        *obj+=bytes[start+i];
    }

    int sign = 1;
    if (*obj / intpow(2,FLOAT_BITS_NBR-1)==1) {
        *obj=(long)*obj%FLOAT_BITS_NBR-1;
        sign = -1;
    }

    int exp = (int)*obj/intpow(2,FLOAT_BITS_MANTISS);
    exp-=16;
    *obj = (long)*obj%intpow(2,FLOAT_BITS_MANTISS);
    if (exp>=0) *obj = sign * (*obj) * intpow(2,exp);
    else *obj = sign * (*obj) / intpow(2,-exp);

    res->object = (object) obj;
    res->bytes = bytes;
    res->start = start+1;

    return res;
}