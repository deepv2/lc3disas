/* 
 * disas.h: header file for disassembly of binary 
 *
 * Copyright (C) 2018 Deepan Venkatesh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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

#include "symbols.h"

uint16_t *saveBinData(const char *path, uint64_t size); // Makes an array for each byte of data in the object file.
// size is the size of the file in bytes
int8_t *getBinaryInstruction(uint16_t *binData, int i); // Loads binary instruction at index i into integer array 
void printHexInstruction(uint16_t *binData, int i); // Prints hex instruction at index i
void printBinaryInstruction(uint16_t *binData, int i); // Prints binary instruction at index i
void le16_to_be16(uint16_t *binData_one_inst); // Switches little endian to big endian on one instruction in binData

void printAssemblyInstruction(uint16_t *binData, int i, symbolList_t *symbols); // prints the translated assembly instruction at index i
// if symbols are not provided, NULL is put as the symbol list

#endif
