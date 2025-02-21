/**
@file encrypt.c
@author Neha Ramesh (nramesh3)
Holds the main function which does the decryption
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

#include "aes.h"
#include "field.h"
#include "io.h"
/** holds the array length*/
#define ARRAY_LENGTH 20
/** holds the min arg length*/
#define MIN_ARG 4
/** holds the second arg */
#define ARG_2 2
/** holds the third arg */
#define ARG_3 3
/** holds the key size */
#define KEY_SIZE 16
/**
 The main function that puts the different parts of the encryption together and writes it 
 to the binary file
 @param argc for the number of arguments
 @param argv for the content of those arguments
 @return EXIT_SUCCESS
 */
int main (int argc, char** argv) {
  char *ip_file = NULL;
  char *op_file = NULL;
  char *key_file = NULL;

  byte *bptr;
  byte *keyptr;
  int bfrsize = 0;
  int keysize = 0;

  if (argc < MIN_ARG) {
    fprintf (stderr,"usage: encrypt <key-file> <input-file> <output-file>\n");
    exit(1);
  }
  for (int i = 1; i < argc; i++){    
    if (i == 1) {
      key_file = argv[1];
      keyptr = readBinaryFile (key_file,&keysize );
      if (keysize != KEY_SIZE){
        fprintf(stderr,"Bad key file: %s\n", key_file);exit(1);
      }
    }
    else if (i == ARG_2) {
      ip_file = argv[2];
    }
    else if (i == ARG_3) {
      op_file = argv[3];
    }
  }
  bptr = readBinaryFile (ip_file,&bfrsize );
  byte prcsArray[ BLOCK_SIZE ];
  int prcs_len = 0;
  byte write_bfr[bfrsize];
  while (prcs_len < bfrsize  ) {
    memcpy(  prcsArray, (bptr + prcs_len), KEY_SIZE * sizeof(byte) );
    encryptBlock( prcsArray, keyptr );
    memcpy(write_bfr + prcs_len, prcsArray, KEY_SIZE * sizeof(byte) );
    prcs_len = prcs_len + KEY_SIZE;
  }
  free (keyptr);
  free (bptr);
  writeBinaryFile ( op_file,write_bfr,bfrsize );
  return EXIT_SUCCESS;
}
