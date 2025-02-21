/**
@file field.c
@author Neha Ramesh (nramesh3)
does the add, subtracting and multiplying of the bytes
*/
#include "field.h"
#include <stdlib.h>
#include <stdio.h>
/** Number of bits in a byte. */
#define BBITS 8
/** holds the reduction poly*/
#define REDUCTION_POLY 0x11B
/** holds the 0x01 value*/
#define BIT_0x01 0x01
/**
 Prints the binary number
 @param number for the number to be printed
 */
void print_binary_s(short number) {
  int num_bits = sizeof(number) * BBITS;
  int print_flag = 0;
  for (int i = num_bits - 1; i >= 0; i--) {
    int bit = (number >> i) & 1;
    if (bit || print_flag) {
      print_flag = 1;
    }
  }
}
/**
 does the galois multiplication to help with fieldMul
 @param a for the first short
 @param b for the second short
 @return the result of the multiplication
 */
short galois_multiply(unsigned short a, unsigned short b) {
  short result = 0;
  for (int i = 0; i < BBITS; i++) {
    print_binary_s(result);
    if (b & BIT_0x01) {
      result = result ^ a;
    }
    a <<= 1;
    b >>= 1;
  }
  return result;
}
/**
 does the galois reduction to help with fieldMul
 @param a for the first short
 @param b for the second short
 @return the result of the reduction
 */
short galois_reduce (unsigned short num) {
  unsigned short reduction_poly = REDUCTION_POLY;  
  reduction_poly = reduction_poly << (BBITS - 1) ;
  print_binary_s(reduction_poly);
  for (int i = BBITS + (BBITS - 1); i >= BBITS; i--) {
    if ((num >> i) & 1) {
      num = num ^ reduction_poly;
    }
    reduction_poly = reduction_poly >> 1;
  }
  return num;
}

/**
 This function performs the addition operation in the 8-bit Galois field used by AES. 
 It adds a and b and returns the result.
 @param a for the first byte to be added
 @param b for the second byte to be added
 @return the result of a and b
 */
byte fieldAdd( byte a, byte b ) 
{
  return a ^ b;
}

/**
 This function performs the subtraction operation in the 8-bit Galois field used by AES. 
 It subtracts b from a and returns the result.
 @param a for the first byte
 @param b for the second byte which a gets subtracted from
 @return the result of the subtraction
 */
byte fieldSub( byte a, byte b )
{
  return a ^ b;

}

/**
 This function performs the multiplication operation in the 8-bit Galois field used by AES. 
 It multiplies a and b and returns the result.
 @param a for the first byte 
 @param b for the second byte
 @return the result
 */
byte fieldMul( byte a, byte b )
{
  unsigned short result = galois_multiply(a, b);
  unsigned short f_result = galois_reduce(result);
  return f_result;

}
