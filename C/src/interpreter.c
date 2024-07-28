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
    *obj = 0;
    for (int i=0; i<INT_BITS_NBR/8; i++) {
        *obj*=256;
        *obj+=bytes.bytes[bytes.start+i];
    }
    if (*obj / intpow(2,INT_BITS_NBR-1)==1) *obj=-(*obj%INT_BITS_NBR-1);

    res.object = (object) obj;
    res.bytes = bytes;
    bytes.start = bytes.start+INT_BITS_NBR/8;

    return res;
}

tuple_obj_bytes nextDouble(bytes bytes) {
    tuple_obj_bytes res;

    double* obj = malloc(sizeof(double));
    *obj = 0;
    for (int i=0; i<FLOAT_BITS_NBR/8; i++) {
        *obj*=256;
        *obj+=bytes.bytes[bytes.start+i];
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

    res.object = (object) obj;
    res.bytes = bytes;
    bytes.start = bytes.start+FLOAT_BITS_NBR/8;

    return res;
}