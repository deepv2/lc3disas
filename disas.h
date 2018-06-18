#ifndef disas_h
#define disas_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

uint16_t *saveBinData(char *path, uint32_t size); // Makes an array for each byte of data in the object file.
// size is the size of the file in bytes

int8_t *getBinaryInstruction(uint16_t *binData, int i); // Loads binary instruction at index i into integer array 

void printInstruction(uint16_t *binData, int i); // Prints binary instruction at index i

#endif
