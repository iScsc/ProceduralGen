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

int pow(int nbr, int exp) {
    if (exp==0) return 1;
    else if (exp==1) return nbr;
    else if (exp%2==0) return pow(nbr*nbr,exp/2);
    else return nbr*pow(nbr,exp-1);
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

    res[0] = sign + (byte) (nbr/(pow(2,(INT_BITS_NBR-8))));
    for (int i=8; i<INT_BITS_NBR; i+=8) {
        res[i/8] = (byte) ((nbr % pow(2,INT_BITS_NBR-i)) / pow(2, INT_BITS_NBR-i-8));
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
    if (nbr<pow(2,FLOAT_BITS_MANTISS)) {
        while (exp>-pow(2,FLOAT_BITS_EXP-1) && nbr<pow(2,FLOAT_BITS_MANTISS-1)) {
            exp-=1;
            nbr*=2;
        }
    }
    else {
        while (exp<pow(2,FLOAT_BITS_EXP-1) && nbr>=pow(2,FLOAT_BITS_MANTISS)) {
            exp+=1;
            nbr/=2;
        }
    }
    exp+=16;

    res[0] = sign + (byte) (exp * pow(2,8-(1+FLOAT_BITS_EXP)) + ((int)nbr)/pow(2,FLOAT_BITS_MANTISS-(8-(1+FLOAT_BITS_EXP))));
    for (int i=8; i<FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP); i+=8) {
        res[i/8] = (byte) ((((int)nbr) % pow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i) / pow(2,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i-8)));
    }

    return res;
}