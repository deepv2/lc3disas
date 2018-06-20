#ifndef disas_h
#define disas_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef _WIN32
  #include <direct.h>
#elif defined __linux__ || defined __APPLE__
  #include <sys/stat.h>
#endif

uint16_t *saveBinData(char *path, uint32_t size); // Makes an array for each byte of data in the object file.
// size is the size of the file in bytes

int8_t *getBinaryInstruction(uint16_t *binData, int i); // Loads binary instruction at index i into integer array 

void printHexInstruction(uint16_t *binData, int i); // Prints hex instruction at index i

void printBinaryInstruction(uint16_t *binData, int i); // Prints binary instruction at index i

void le16_to_be16(uint16_t *binData_one_inst); // Switches little endian to big endian on one instruction in binData



void printAssemblyInstruction(uint16_t *binData, int i); // prints the translated assembly instruction at index i

#endif
