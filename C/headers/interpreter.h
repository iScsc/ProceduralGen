/**
 * @file interpreter.h
 * @author BlueNZ
 * @brief Header to interpreter functions
 * @version 0.1
 * @date 2024-07-27
 * 
 */

#ifndef INTERP
#define INTERP

#include "map.h"

// ------- Structure definition ------- //

typedef char byte;
typedef char* object; //an alias for pointers

typedef struct tuple_obj_bytes {
    object object; 
    byte* bytes;
    int start;
} tuple_obj_bytes;


// ------- Constants ------- //

#define BYTE_TRUE (byte) 1
#define BYTE_FALSE (byte) 0
#define INT_BITS_NBR 24 //24 should be a multiple of 8
#define FLOAT_BITS_EXP 5 //5 should be < 8
#define FLOAT_BITS_MANTISS 18 
#define FLOAT_BITS_NBR 1 + FLOAT_BITS_EXP + FLOAT_BITS_MANTISS //24 should be a multiple of 8

#define BYTES_VERSION (byte) 0



// ------- Functions ------- //

int pow(int nbr, int exp);

byte* bytesUint8(__uint8_t nbr);

byte* bytesInt(int nbr);

byte* bytesDouble(double nbr);



tuple_obj_bytes* nextUint8(int start, byte* bytes);

tuple_obj_bytes* nextInt(int start, byte* bytes);

tuple_obj_bytes* nextDouble(int start, byte* bytes);






#endif