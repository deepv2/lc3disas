#include "disas.h"

uint16_t *saveBinData(char *path, uint32_t size) {
	FILE *bin = fopen(path, "rb");
	uint16_t *binData = malloc((size / 2) * sizeof(uint16_t));

	if(fread(binData, 2, size, bin) != size / 2)
		return NULL;
	// If data was unable to be read 16 bytes each, return NULL

	// TODO: Switch endianness
	fclose(bin); 
	return binData;
}

int8_t *getBinaryInstruction(uint16_t *binData, int i) {
	int8_t *inst = malloc(17 * sizeof(char));
	uint16_t data = binData[i];
	int j;
	inst[0] = i; // embed number of instruction in binary instruction
	for(j = 16; j >= 1; j--) {
		if(data & 0x0001) // if lsb in selected partition is 1
			inst[j] = 1;
		else
			inst[j] = 0;
		data = data >> 1; // right shift one to iterate all 8 bits
	}
	return inst;
}

void printInstruction(uint16_t *binData, int i) {
	printf("%d: ", i); // print index of instruction
	int j;
		printf("%04x", binData[i]);
	printf("\n");
}
