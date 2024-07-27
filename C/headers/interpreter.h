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


// ------- Constants ------- //

#define BYTE_TRUE (byte) 1
#define BYTE_FALSE (byte) 0
#define INT_BITS_NBR 24 //24 should be a multiple of 8
#define FLOAT_BITS_EXP 5 //5 should be < 8
#define FLOAT_BITS_MANTISS 18 
#define FLOAT_BITS_NBR 1 + FLOAT_BITS_EXP + FLOAT_BITS_MANTISS //24 should be a multiple of 8

#define BYTES_VERSION (byte) 0



#endif