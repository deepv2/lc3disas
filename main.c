#include "disas.h"
#include "symbols.h"

int printError();

int main(int argc, char *argv[]) {
	if(argc == 3 || argc == 4) {
		// check if [.obj file path] is actually an .obj file

		unsigned int length = strlen(argv[2]);
		char *obj = argv[2];
		if(obj[length - 3] != 'o' || obj[length - 2] != 'b' || obj[length - 1] != 'j') return printError();

		// check if [.sym file path] is actually a .sym file if one is provided

		if(argc == 4) {
			unsigned int length = strlen(argv[3]);
			char *obj = argv[3];
			if(obj[length - 3] != 's' || obj[length - 2] != 'y' || obj[length - 1] != 'm') return printError();
		}

		// getting size of .obj file	
		struct stat st;
		stat(argv[2], &st);
		uint32_t size = st.st_size;

		uint16_t *data = saveBinData(argv[2], size);
		symbolList_t *symbols;
		if(argc == 4) symbols = readSymbols(argv[3]);
		else symbols = NULL;
		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--hex")) {
			for(int i = 0; i < size / 2; i++) {
				printHexInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-b") || !strcmp(argv[1], "--bin")) {
			for(int i = 0; i < size / 2; i++) {
				printBinaryInstruction(data, i);
			}
		}
		else if(!strcmp(argv[1], "-a") || !strcmp(argv[1], "--asm")) {
			for(int i = 0; i < size / 2; i++) {
				printAssemblyInstruction(data, i, symbols);	
			}
			printf(".END\n");
		} 
		else {
			free(data);
			deallocateSymbols(symbols);
			return printError();
		}
		free(data);
		deallocateSymbols(symbols);
		return 0;
	} else {
		return printError();
	}
}

int printError() {
		printf("Usage: lc3disas [OPTION] [.obj file path] [.sym file path].\n");
		printf("-h, --hex		Prints out instructions in hexadecimal format.\n");
		printf("-b, --bin		Prints out instructions in binary format.\n");
		printf("-a, --asm		Prints out instructions in assembly format.\n");
		printf(".sym file is optional. Labels will not be shown if .sym file not provided.\n");
		return 1;
}
