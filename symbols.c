/* 
 * symbols.c: source code to handle the input of symbols
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

#include "symbols.h"

symbolList_t *readSymbols(const char *path) {
	FILE *fp = fopen(path, "r");
	if(fp == NULL) return NULL; // if file cannot be opened, return NULL
	char buffer[255];
	uint8_t length; // buffer and length of string in buffer
	uint16_t addressBuffer; // buffer for address

	symbolList_t *symbols = malloc(sizeof(symbolList_t));
	symbolList_t *head = symbols, *prev;

	fseek(fp, 105, SEEK_SET); // fixed file format, skip words until first symbol
	while(1) {
		if(fscanf(fp, "%s %hx", buffer, &addressBuffer) != 2) { // if at EOF
			free(symbols);
			prev->next = NULL; // free previously-allocated next node, remove dangling pointer and break
			break;
		}
		length = strlen(buffer);

		symbols->label = malloc(sizeof(char) * (length + 1)); // allocate variable in symbolList and the list itself
		strncpy(symbols->label, buffer, length + 1);
		symbols->addr = addressBuffer;
		symbols->next = malloc(sizeof(symbolList_t)); // allocate symbolList only if current pointer is null

		prev = symbols;
		symbols = symbols->next; // put label values into list and move to the next item in list
		fseek(fp, 4, SEEK_CUR); // skip slashed, tab, and newline after each label is read
	}
	fclose(fp);
	return head;
}

void deallocateSymbols(symbolList_t *head) {
	symbolList_t *prev = head;
	while(head != NULL) {
		free(head->label);
		head = head->next;
		free(prev);
		prev = head;
	}
}
char *findLabel(uint16_t address, symbolList_t *symbols) {
	if(symbols == NULL) return NULL; // base case
	else if(symbols->addr == address) return symbols->label;
	else if(symbols->addr > address) return NULL; // optimization since symbolList is read in increasing address order
	else return findLabel(address, symbols->next);
}
