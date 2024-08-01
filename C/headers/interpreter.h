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

// ------- Structure definition ------- //

typedef unsigned char byte; //type used as bytes
typedef char* object; //an alias for pointers

/**
 * @brief a simple implementation of byte strings
 */
typedef struct bytes {
    byte* bytes; // the byte array
    int size; // to keep track of the size of the byte array
    int start; // where to look for the current byte in the array
} bytes;

/**
 * @brief a placeholder to return both an object (a number or a struct) and a byte string (bytes)
 */
typedef struct tuple_obj_bytes {
    object object; // a pointer to the object
    bytes bytes; // the byte string
} tuple_obj_bytes;


// ------- Constants ------- //

#define BYTE_TRUE (byte) 1
#define BYTE_FALSE (byte) 0
#define INT_BITS_NBR 24 //24 should be a multiple of 8
#define FLOAT_BITS_EXP 5 //5 should be < 8
#define FLOAT_BITS_MANTISS 18 
#define FLOAT_BITS_NBR (1 + FLOAT_BITS_EXP + FLOAT_BITS_MANTISS) //24 should be a multiple of 8

#define GRID_ENCODING (byte) 1
#define LAYER_ENCODING (byte) 2
#define CHUNK_ENCODING (byte) 3
#define MAP_ENCODING (byte) 4
#define COMPLETE_MAP_ENCODING (byte) 5

#define BYTES_VERSION (byte) 0



// ------- Functions ------- //

/**
 * @brief 'Fast' int powers.
 * 
 * @param nbr (int) : the nbr to be exponantiated
 * @param exp (int) : the exponant
 * @return int : the result of nbr^exp.
 */
int intpow(int nbr, int exp);



/**
 * @brief Deallocates the byte array of a bytes struct
 * 
 * @param b (bytes) : the bytes struct
 */
void freeBytes(bytes b);

/**
 * @brief Auxiliary function to print bytes (update a string with the hexadecimal representation of a byte).
 * 
 * @param c (unsigned char) : the byte to print
 * @param res (char*) : the string to update
 * @return char* : the updated string
 */
char* hex(unsigned char c, char* res);

/**
 * @brief Prints a bytes struct
 * 
 * @param bytes (bytes) : the bytes to print
 * @param start (char*) : a string to print beforehand
 * @param end (char*) : a string to print after ("\n" for instance)
 */
void printBytes(bytes bytes, char* start, char* end);

/**
 * @brief Updates a byte string starting from a given index with the values of another byte string
 * 
 * @param b (bytes) : the bytes to update
 * @param bb (bytes) : the bytes to use for updating
 * @param start (int) : the starting index in b
 */
void concatBytes(bytes b, bytes bb, int start);



/**
 * @brief Encodes an unsigned 8bits int.
 * 
 * @param nbr (__uint8_t) : the unsigned int.
 * @return bytes : the byte string representing the encoded number.
 */
bytes bytesUint8(__uint8_t nbr);

/**
 * @brief Encodes an int.
 * 
 * @param nbr (int) : the int.
 * @return bytes : the byte string representing the encoded number.
 */
bytes bytesInt(int nbr);

/**
 * @brief Encodes a double.
 * 
 * @param nbr (double) : the double.
 * @return bytes : the byte string representing the encoded number.
 */
bytes bytesDouble(double nbr);



/**
 * @brief Decodes an encoded unsigned 8bits int.
 * 
 * @param bytes (bytes) : the encoded byte string.
 * @return tuple_obj_bytes : the byte string with updated start index and a pointer to the number decoded.
 */
tuple_obj_bytes nextUint8(bytes bytes);

/**
 * @brief Decodes an encoded int.
 * 
 * @param bytes (bytes) : the encoded byte string.
 * @return tuple_obj_bytes : the byte string with updated start index and a pointer to the number decoded.
 */
tuple_obj_bytes nextInt(bytes bytes);

/**
 * @brief Decodes an encoded double.
 * 
 * @param bytes (bytes) : the encoded byte string.
 * @return tuple_obj_bytes : the byte string with updated start index and a pointer to the number decoded.
 */
tuple_obj_bytes nextDouble(bytes bytes);






#endif