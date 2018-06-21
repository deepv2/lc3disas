#include "disas.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Format: lc3disas [FILE PATH]. Supports one file per program launch.\n");
		return 1;
	} else {
		// POSIX compliant way of geting size of file	
		struct stat st;
		stat(argv[1], &st);
		uint32_t size = st.st_size;

		uint16_t *data = saveBinData(argv[1], size);

		for(int i = 0; i < size / 2; i++) {
			printf("%d: ", i + 1);
			printAssemblyInstruction(data, i);			
		}
		free(data);
		return 0;
	}
}
