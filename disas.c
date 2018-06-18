#include "disas.h"

uint16_t *saveBinData(char *path, uint32_t size) {
	FILE *bin = fopen(path, "rb");
	uint16_t *binData = malloc((size / 2) * sizeof(uint16_t));
	int i;

	if(fread(binData, 2, size, bin) != size / 2)
		return NULL;
	// If data was unable to be read 16 bytes each, return NULL

	fclose(bin);
	for(i = 0; i < size / 2; i++)
		le16_to_be16(&binData[i]);
	return binData;
}

int8_t *getBinaryInstruction(uint16_t *binData, int i) {
	int8_t *inst = malloc(16 * sizeof(char));
	uint16_t data = binData[i];
	int j;
	for(j = 15; j >= 0; j--) {
		if(data & 0x0001) // if lsb in selected partition is 1
			inst[j] = 1;
		else
			inst[j] = 0;
		data = data >> 1; // right shift one to iterate all 8 bits
	}
	return inst;
}

void printHexInstruction(uint16_t *binData, int i) {
	printf("%d: ", i); // print index of instruction
	printf("%04x", binData[i]);
	printf("\n");
}
void printBinaryInstruction(uint16_t *binData, int i) {
	printf("%d: ", i); // print index of instruction
	int j;
	int8_t *inst = getBinaryInstruction(binData, i);
	for(j = 0; j < 16; j++)
		printf("%d", inst[j]);
	printf("\n");
	free(inst);
}	

void le16_to_be16(uint16_t *binData_one_inst) {
	uint16_t right_half = *binData_one_inst & 0x00FF;
	uint16_t left_half = *binData_one_inst & 0xFF00;
	right_half = right_half << 8; // Right goes to left and left goes to right halves of 16-bit width
	left_half = left_half >> 8;
	*binData_one_inst = right_half + left_half;
}
	
