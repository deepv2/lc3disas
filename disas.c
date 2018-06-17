#include "disas.h"

uint8_t *saveBinData(char *path) {
	FILE *bin = fopen(path, "rb");
	uint32_t size;
	int i;

	// POSIX compliant way of geting size of file	
	struct stat st;
	stat(path, &st);
	size = st.st_size;

	uint8_t *binData = malloc(size * sizeof(uint8_t));

	for(i = 0; i < size; i++) {
		binData[i] = fgetc(bin);
	}
	fclose(bin); 
	return binData;
}

char *getBinaryInstruction(uint8_t *binData, int i) {
	char *inst = malloc(16 * sizeof(char));
	uint8_t msb15_8 = binData[i];
	uint8_t lsb7_0 = binData[i + 1];
	int j;
	for(j = 7; j >= 0; j--) {
		if(msb15_8 & 0x01) // if lsb in selected partition is 1
			inst[j] = '1';
		else
			inst[j] = '0';
		msb15_8 = msb15_8 >> 1; // right shift one to iterate all 8 bits
	}
	// repeat for lsb 8 bits
	for(j = 15; j >= 8; j--) {
		if(lsb7_0 & 0x01)
			inst[j] = '1';
		else
			inst[j] = '0';
		lsb7_0 = lsb7_0 >> 1;
	}
	return inst;
}

void dumpInstructions(uint8_t *binData) {
	//int8_t binaryBuffer[16];
}
