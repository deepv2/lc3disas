#ifndef disas_h
#define disas_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>

uint8_t *saveBinData(char *path); // Makes an array for each byte of data in the object file.

char *getBinaryInstruction(uint8_t *binData, int i); // Loads binary instruction at index i and i + 1 into string

void dumpInstructions(uint8_t *binData); // Prints each instruction to the console

#endif
