#include "disas.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Format: lc3disas [FILE PATH]. Supports one file per program launch.\n");
		return 1;
	} else {
		uint8_t *data = saveBinData(argv[1]);
		int i;
		for(i = 0; i < 8; i += 2) {
			char *test = getBinaryInstruction(data, i);
			printf("%d: %s\n", i / 2, test);
			free(test);
		}
		printf("\n");
	}
}
