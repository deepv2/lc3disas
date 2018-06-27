/* 
 * symbols.h: header file to handle the input of symbols
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

#ifndef symbols_h
#define symbols_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct symbolList {
	uint16_t addr;
	char *label;
	struct symbolList *next;
} symbolList_t;

symbolList_t *readSymbols(const char *path); // gets symbols and addresses and puts into linked list
void deallocateSymbols(symbolList_t *head); // deallocates all values in the symbolList
char *findLabel(uint16_t address, symbolList_t *symbols); // finds label associated with address given, returns NULL if not found

#endif
