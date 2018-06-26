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

#endif
