#include "disas.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Format: lc3disas [FILE PATH]. Supports one file per program launch.\n");
		return 1;
	} else {
		FILE *obj = open_file(argv[1]);
		int hex;
		while((hex = fgetc(obj)) != EOF)		
			printf("%02x ", hex);
		printf("\n");
	}
}
