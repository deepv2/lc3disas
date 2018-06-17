#include "disas.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Format: lc3disas [FILE PATH]. Supports one file per program launch.\n");
		return 1;
	} else {
		FILE *obj = fopen(argv[1], "r");
		uint8_t *data = saveBinData(argv[1]);
//		printf("%02x\n", data[0]);
		int i;
		for(i = 0; i < 8; i += 2) {
			char *test = getBinaryInstruction(data, i);
			printf("%s\n", test);
			free(test);
		}
/*		uint8_t hex;
		while((hex = fgetc(obj)) != 255)
			printf("%02x ", hex);
*/		uint8_t hex = fgetc(obj);
		while(!feof(obj)) {
			printf("%02x ", hex);
			hex = fgetc(obj);
		}		

//		printf("%u", (uint8_t)sizeof(hex));
		printf("\n");
	}
}
