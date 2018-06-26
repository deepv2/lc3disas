#include "disas.h"
#include "symbols.h"

int main(int argc, char *argv[]) {
	if(argc == 2 /*|| argc == 3*/) {
		// getting size of file	
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
	} else {
		printf("Format: lc3disas [.obj file path] <.sym file path>. Supports one file per program launch.\n");
		printf(".sym file is optional. Labels will not be shown if .sym file not provided, but .obj file is required.\n");
		return 1;
	}
}
