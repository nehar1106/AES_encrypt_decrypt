/**
@file aes.c
@author Neha Ramesh (nramesh3)
Does all the different byte operations with the squares and blocks
*/
#include "aes.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "field.h"


/**
   Return the sBox substitution value for a given byte value.
   @param v byte input value.
   @return substitution for the given byte.
*/
static byte substBox( byte v )
{
  // Forward-direction replacement map for the sBox.
  static const byte rule[] =
    { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
      0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
      0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
      0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
      0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
      0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
      0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
      0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
      0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
      0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
      0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
      0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
      0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
      0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
      0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
      0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
      0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
      0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
      0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
      0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
      0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
      0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
      0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
      0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
      0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
      0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
      0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
      0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
      0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
      0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
      0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
      0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 };

  // Return the replaceent for v based on the map.
  return rule[ v ];
}

/**
   Return the inverse sBox substitution value for a given byte value.
   @param v byte input value.
   @return inverse substitution for the given byte.
*/
static byte invSubstBox( byte v )
{
  // Inverse-direction replacement map for the sBox.
  static const byte irule[] =
  { 0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
    0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
    0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
    0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
    0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
    0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
    0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
    0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
    0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
    0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
    0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
    0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
    0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
    0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
    0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D };
  
  // Return the (inverse) replaceent for v based on the map.
  return irule[ v ];
}


/**
 This function performs the mixColumns operation on the given 4 × 4 square of values, 
 multiplying each column by the matrix shown in the requirements section.
 @param square for the square of values
 */
void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{
  // Matrix by which each column of square is multiplied.
  static const byte mixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ] = {
    { 0x02, 0x03, 0x01, 0x01 },
    { 0x01, 0x02, 0x03, 0x01 },
    { 0x01, 0x01, 0x02, 0x03 },
    { 0x03, 0x01, 0x01, 0x02 }
  };

  // ...
  byte new_square[BLOCK_ROWS][BLOCK_COLS];
  for (int c = 0; c < BLOCK_COLS; c++) {
    for (int r = 0; r < BLOCK_ROWS; r++) {
      new_square[r][c] = 0;
      for (int k = 0; k < BLOCK_COLS; k++) {
        new_square[r][c] ^= fieldMul(mixMatrix[r][k], square[k][c]);
      }
    }
  }
  memcpy(square, new_square, BLOCK_ROWS * BLOCK_COLS);

}
/**
 This function performs the inverse of the mixColumns operation on the given 4 × 4 square of values, 
 multiplying each column by the inverse of the mixColumns matrix shown in the requirements section.
 @param square for the square of values
 */
void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{
  // Matrix by which each column of square is multiplied.
  static const byte invMixMatrix[ BLOCK_ROWS ][ BLOCK_COLS ] = {
    { 0x0E, 0x0B, 0x0D, 0x09 },
    { 0x09, 0x0E, 0x0B, 0x0D },
    { 0x0D, 0x09, 0x0E, 0x0B },
    { 0x0B, 0x0D, 0x09, 0x0E }
  };

  // ...
  byte new_square[BLOCK_ROWS][BLOCK_COLS];
  for (int c = 0; c < BLOCK_COLS; c++) {
    for (int r = 0; r < BLOCK_ROWS; r++) {
      new_square[r][c] = 0;
        for (int k = 0; k < BLOCK_COLS; k++) {
          new_square[r][c] ^= fieldMul(invMixMatrix[r][k], square[k][c]);
        }
     }
  }
  memcpy(square, new_square, BLOCK_ROWS * BLOCK_COLS);
}


void xor_words(byte *dst, const byte *src1, const byte *src2, int len) 
{
  for (int i = 0; i < len; i++) {
    dst[i] = src1[i] ^ src2[i];
  }
}
/**
 This function computes the g function used in generating the subkeys from the original, 16-byte key. 
 It takes a 4-byte input via the src parameter and returns a 4-byte result via the dest parameter. 
 The value, r, gives the round number, between 1 and 10 (inclusive).
 @param dest for the destination of the output 
 @param src for the source of the input
 @param r for the round number
 */
void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r )
{
  // Constant values used in each round of the g function.
  static const byte roundConstant[ ROUNDS + 1 ] = {
  0x00, // First element not used.
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
  };

    dest[0] = substBox(src[1]) ^ roundConstant[r] ;  

    dest[1] = substBox(src[2]);  

    dest[2] = substBox(src[3]);  

    dest[3] = substBox(src[0]);  
}  
/**
 This function fills in the subkey array with subkeys for each round of AES, computed from the given key.
 @param subkey for the array to be filled
 @param key for the key given
 */
void generateSubkeys(byte subkey[ROUNDS + 1][BLOCK_SIZE], byte const key[BLOCK_SIZE]) 
{

  byte new_g_subkey[WORD_SIZE];
  new_g_subkey[0] = '\0';
  for (int i = 0; i < ROUNDS + 1; i++) {
    subkey[i][0] = '\0';
  }

  memcpy(subkey[0], key, BLOCK_SIZE);
  for (int i = 1; i < ROUNDS + 1; i++) {
    gFunction(new_g_subkey, &subkey[i - 1][12], i);
    xor_words(&subkey[i][0], &subkey[i - 1][0], new_g_subkey, 4);
    xor_words(&subkey[i][4], &subkey[i - 1][4], &subkey[i][0], 4);
    xor_words(&subkey[i][8], &subkey[i - 1][8], &subkey[i][4], 4);
    xor_words(&subkey[i][12], &subkey[i - 1][12], &subkey[i][8], 4);
  }

}

/**
 This function performs the addSubkey operation, adding the given subkey (key) to the given data array.
 @param data for the array that the subkey should be added to 
 @param key for the given subkey
 */
void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] )
{
  for (int i = 0; i < BLOCK_SIZE; i++) {
    data[i] ^= key[i];
  }
}
/**
 This function rearranges a block of 16 data values from the one-dimensional arrangement to the 4 × 4 arrangement.
 @param square for the square that the block should be arranged to
 @param data for the given block
 */
void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ])
{
  for (int i = 0; i < BLOCK_SIZE; i++) {
    int r1 = i % BLOCK_ROWS;
    int c1 = i / BLOCK_ROWS;
    square[r1][c1] = data[i];
  }
}
/*
This function rearranges a 4 × 4 arrangement of data values,
returning them as a one-dimensional array of 16 values.
@param data for the block one dimensional array of values
@param square for the square arrangement of data values that have to be rearranged
*/
void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{

  for (int r1 = 0; r1 < BLOCK_ROWS; r1++) {
    for (int c1 = 0; c1 < BLOCK_COLS; c1++) {
      int index = c1 * BLOCK_ROWS + r1;
      data[index] = square[r1][c1];
    }
  }
}
/**
This function performs the shiftRows operation on the given 4 × 4 square of values.
@param square for the square of values that the shift rows operation should be performed on
*/
void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{

  byte tmp1;
  for (int r1 = 1; r1 < BLOCK_ROWS; r1++) {
    for (int shift = 0; shift < r1; shift++) {
      tmp1 = square[r1][0];
      for (int c1 = 0; c1 < BLOCK_COLS - 1; c1++) {
        square[r1][c1] = square[r1][c1 + 1];
      }
      square[r1][BLOCK_COLS - 1] = tmp1;
    }
  }
}
/**
This function performs the inverse shiftRows operation on the given 4 × 4 square of values.
@param square for the square of values that the inverse shift rows operation should be performed on
*/
void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] )
{

  byte tmp1;
  for (int r1 = 1; r1 < BLOCK_ROWS; r1++) {
    for (int shift = 0; shift < r1; shift++) {
      tmp1 = square[r1][BLOCK_COLS - 1];
      for (int c1 = BLOCK_COLS - 1; c1 > 0; c1--) {
        square[r1][c1] = square[r1][c1 - 1];
      }
      square[r1][0] = tmp1;
    }
  }
}
/**
 This function encrypts a 16-byte block of data using the given key. 
 It generates the 11 subkeys from key, adds the first subkey, 
 then performs the 10 rounds of operations needed to encrypt the block
 @param data for the data to encrypt
 @param key for the key that provides the subkeys
 */
void encryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]) {

  byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ];
  byte data_subst[BLOCK_SIZE];
  byte square[ BLOCK_ROWS ][ BLOCK_COLS ];
  generateSubkeys( subkey, key);
  addSubkey (data, subkey[0]);
  for (int r1 = 1; r1 <= 10; r1++ ) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      data_subst[j] = substBox(data[j]); 
    }
    blockToSquare(square, data_subst);
    shiftRows( square );
    if (r1 < 10) {
      mixColumns( square );
    }
    squareToBlock(data,square);
    addSubkey (data, subkey[r1]);
  }
}
/**
 This function decrypts a 16-byte block of data using the given key. 
 It generates the 11 subkeys from key then performs the 10 rounds of  inverse operations
 needed to decrypt the block and then adds the subkey
 @param data for the data to decrypt
 @param key for the key that provides the subkeys
 */
void decryptBlock(byte data[BLOCK_SIZE], byte key[BLOCK_SIZE]) {
  byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ];
  byte square[ BLOCK_ROWS ][ BLOCK_COLS ];
  generateSubkeys( subkey, key);
  for (int r1 = 10; r1 >= 1; r1-- ) {
    addSubkey (data, subkey[r1]);
    blockToSquare(square, data);
    if (r1 < 10) {
      unMixColumns( square );
    }
    unShiftRows( square );
    squareToBlock(data,square);
    for (int j = 0; j < BLOCK_SIZE; j++) {
      data[j] = invSubstBox(data[j]); 
    }
  }
  addSubkey (data, subkey[0]);
}

