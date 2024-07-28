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

typedef struct bytes {
    byte* bytes;
    int size;
    int start;
} bytes;

typedef struct tuple_obj_bytes {
    object object; 
    bytes bytes;
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

int intpow(int nbr, int exp);

bytes bytesUint8(__uint8_t nbr);

bytes bytesInt(int nbr);

bytes bytesDouble(double nbr);



tuple_obj_bytes nextUint8(bytes bytes);

tuple_obj_bytes nextInt(bytes bytes);

tuple_obj_bytes nextDouble(bytes bytes);






#endif