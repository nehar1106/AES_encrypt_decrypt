/**
@file io.c
@author Neha Ramesh (nramesh3)
Reads the lines in the binary file and writes them to a binary file
*/
#include "stdlib.h"
#include "stdio.h"
#include "field.h"
#include "io.h"
/**
 This function reads the contents of the binary file with the given name.
 @param filename for the name of the binary file to read
 @param size for the size of the file
 @return pointer to a dynamically allocated array of bytes containing the entire file contents.
 */
byte *readBinaryFile( char const *filename, int *size )
{
  FILE *file = fopen(filename, "rb");
  if (!file) {
	fprintf(stderr,"Can't open file: %s\n", filename);
	exit(1);
  }
  fseek(file, 0, SEEK_END);
  *size = ftell(file);
  fseek(file, 0, SEEK_SET);
  byte *buffer = (byte *)malloc(*size + 1);
  if (!buffer) {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }
  size_t bytes_read = fread(buffer, 1, *size, file);
  if (bytes_read != *size) {
    perror("Error reading file");
    free(buffer);
    fclose(file);
    return NULL;
  }
  fclose(file);
  return buffer;
}
/**
 This function writes the contents of the given data array (in binary) to the file with the given name.
 @param filename for the name of the file to write to
 @param data for the data that is to be written to the file
 @param size for the size of the file
 */
void writeBinaryFile( char const *filename, byte *data, int size )
{
  FILE *file = fopen(filename, "wb");
  if (!file) {
	fprintf(stderr,"Can't open file: %s\n", filename);
	exit(1);
  }
  size_t bytes_written = fwrite(data, 1, size, file);
  if (bytes_written != size) {
    perror("Error writing file");
  }
  fclose(file);
}