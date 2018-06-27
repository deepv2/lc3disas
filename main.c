/* 
 * main.c: main executable loop for disassembly of LC3 binary with or without symbols
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

#include "disas.h"
#include "symbols.h"

int printError();

int main(int argc, char *argv[]) {
	if(argc == 3 || argc == 4) {
		// check if [.obj file path] is actually an .obj file

		unsigned int length = strlen(argv[2]);
		char *obj = argv[2];
		if(obj[length - 3] != 'o' || obj[length - 2] != 'b' || obj[length - 1] != 'j') {
			printf("error: first file given is not a .obj file\n");
			return printError();
		}

		// check if [.sym file path] is actually a .sym file if one is provided
		if(argc == 4) {
			unsigned int length = strlen(argv[3]);
			char *obj = argv[3];
			if(obj[length - 3] != 's' || obj[length - 2] != 'y' || obj[length - 1] != 'm') {
				printf("error: second file given is not a .sym file\n");
				return printError();
			}
		}

		// getting size of .obj file	
		struct stat st;
		stat(argv[2], &st);
		if(st.st_size == 0) { // check if size is 0 bytes (file does not exist)
			printf("error: cannot get size of .obj file\n");
			return printError();
		}

		uint16_t *data = saveBinData(argv[2], st.st_size);
		if(data == NULL) { // check if file cannot be opened
			printf("error: cannot open .obj file\n");
			return printError();
		}

		symbolList_t *symbols;
		if(argc == 4) {
			 symbols = readSymbols(argv[3]);
			 if(symbols == NULL) {
				 printf("error: cannot open .sym file\n");
				 return printError();
			 }
		}
		else symbols = NULL;
		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--hex")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				printHexInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-b") || !strcmp(argv[1], "--bin")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				printBinaryInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-a") || !strcmp(argv[1], "--asm")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				printAssemblyInstruction(data, i, symbols);	
			}
			printf(".END\n");
		} 
		else if(!strcmp(argv[1], "-hi") || !strcmp(argv[1], "--hex-indexed")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				if(i != 0) printf("0x%04X: ", data[0] + i - 1);
				else printf("(.ORIG) ");
				printHexInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-bi") || !strcmp(argv[1], "--bin-indexed")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				if(i != 0) printf("0x%04X: ", data[0] + i - 1);
				else printf("(.ORIG) ");
				printBinaryInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-ai") || !strcmp(argv[1], "--asm-indexed")) {
			for(int i = 0; i < st.st_size / 2; i++) {
				if(i != 0) printf("0x%04X: ", data[0] + i - 1);
				printAssemblyInstruction(data, i, symbols);	
			}
			printf(".END\n");
		}
		else {
			printf("error: option not recognized\n");
			free(data);
			deallocateSymbols(symbols);
			return printError();
		}
		free(data);
		deallocateSymbols(symbols);
		return 0;
	} else {
		printf("error: too many or too little arguments given\n");
		return printError();
	}
}

int printError() {
		printf("Usage: lc3disas [OPTION] [.obj file path] [.sym file path].\n");
		printf("-h, --hex		Prints out instructions in hexadecimal format.\n");
		printf("-b, --bin		Prints out instructions in binary format.\n");
		printf("-a, --asm		Prints out instructions in assembly format.\n");
		printf("-hi, --hex-indexed	Prints out instructions in hexadecimal format with memory indices.\n");
		printf("-bi, --bin-indexed	Prints out instructions in binary format with memory indices.\n");
		printf("-ai, --asm-indexed	Prints out instructions in assembly format with memory indices.\n");
		printf(".sym file is optional. Labels will not be shown if .sym file is not provided.\n");
		return 1;
}
