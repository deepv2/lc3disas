#include "disas.h"

uint16_t *saveBinData(const char *path, uint32_t size) {
	FILE *bin = fopen(path, "rb");
	if(bin == NULL) return NULL; // if file cannot be opened, return NULL
	uint16_t *binData = malloc((size / 2) * sizeof(uint16_t));

	if(fread(binData, 2, size, bin) != size / 2)
		return NULL;
	// If data was unable to be read 16 bytes each, return NULL

	fclose(bin);
	for(int i = 0; i < size / 2; i++)
		le16_to_be16(&binData[i]);
	return binData;
}

int8_t *getBinaryInstruction(uint16_t *binData, int i) {
	int8_t *inst = malloc(16 * sizeof(char));
	uint16_t data = binData[i];
	for(int j = 15; j >= 0; j--) {
		if(data & 0x0001) // if lsb in selected partition is 1
			inst[j] = 1;
		else
			inst[j] = 0;
		data = data >> 1; // right shift one to iterate all 8 bits
	}
	return inst;
}

void printHexInstruction(uint16_t *binData, int i) {
	printf("0x%04x\n", binData[i]);
}
void printBinaryInstruction(uint16_t *binData, int i) {
	int8_t *inst = getBinaryInstruction(binData, i);
	for(int j = 0; j < 16; j++)
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
	
void printAssemblyInstruction(uint16_t *binData, int i, symbolList_t *symbols) {
	int8_t *numberData = getBinaryInstruction(binData, i);
	uint16_t instruction = binData[i];

	uint8_t DR, SR1, SR2, BaseR; 	// Destination Register, Source Register 1 and 2, BaseR for LD/ST operations (R1-R7)
	int8_t offset; 			// Offset for ADD/AND/STR/LDR
	int16_t PCoffset; 		// PC Offset for LD/ST operations, LEA, JSR, BR


	if(i == 0) { 	// Support for .ORIG at the start of the program
		printf(".ORIG x%04X\n", instruction);
		free(numberData);
		return;
	}

	char *label;
	if((label = findLabel(binData[0] + i - 1, symbols)) != NULL) printf("%s ", label);
	// prints label in front of instruction if it applies

	// Moderate support for finding assembler directives based on common traits
	if((instruction & 0xFF00) == 0)  {
		/* if data was only in lower bytes, most likely an ASCII character or number.
		but, since there is no way to know which one it is, we will assume it is an
		ASCII character, since numbers can be put into registers with ADD anyways.
		*/
		printf(".FILL '%c'\n", instruction);
		free(numberData);
		return;
	}

	switch(instruction & 0xF000)	// Find which opcode is present
	{
	case(0x1000): // ADD
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		SR1 = 4 * numberData[7] + 2 * numberData[8] + numberData[9];

		// Algorithm to convert to negative number:
		// if MSB in offset is 1, sign extend the 1, overwriting the rest of the instruction
		// else, sign extend 0's to the rest of the instruction

		if(numberData[10]) { 		// Add imm5 mode if 1
			if(numberData[11]) {	// if MSB is 1 (negative offset)
				offset = (int8_t)(instruction | 0xFFE0);
			} else {
				offset = (int8_t)(instruction & 0x001F);
			}
			printf("ADD R%d, R%d, #%d\n", DR, SR1, offset);
		} else { 			// Add register mode if 0
			SR2 = 4 * numberData[13] + 2 * numberData[14] + numberData[15];
			printf("ADD R%d, R%d, R%d\n", DR, SR1, SR2);
		}
		break;
	case(0x5000): // AND
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		SR1 = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
		if(numberData[10]) { 	 	// And imm5 mode if 1
			if(numberData[11]) {	// if MSB is 1 (negative offset)
				offset = (int8_t)(instruction | 0xFFE0);
			} else {
				offset = (int8_t)(instruction & 0x001F);
			}
			printf("AND R%d, R%d, #%d\n", DR, SR1, offset);
		} else {		 	// And register mode if 0
			SR2 = 4 * numberData[13] + 2 * numberData[14] + numberData[15];
			printf("AND R%d, R%d, R%d\n", DR, SR1, SR2);
		}
		break;
	case(0x0000): // BR
		printf("BR");
		if(numberData[4]) // printing condition codes n/z/p if bit is 1
			printf("n");	
		if(numberData[5])
			printf("z");	
		if(numberData[6])
			printf("p");

		if(numberData[7]) // if MSB is 1 (negative offset)
			PCoffset = instruction | 0xFE00;
		else 		  // if MSB is 0 (positive offset)
			PCoffset = instruction & 0x01FF;
		
		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {					
			// if symbols exist, see if one is given for our instruction and print that
			printf(" %s\n", label);
		}
		// if no label, print out manual PCoffset
		else if(PCoffset < 0)
			printf(" PC%d\n", PCoffset);
		else if(PCoffset == 0)
			printf(" PC\n");
		else
			printf(" PC+%d\n", PCoffset);
		break;
	case(0xC000): // JMP
		BaseR = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
		if(BaseR == 7) // Support for RET (JMP R7)
			printf("RET\n");
		else
			printf("JMP R%d\n", BaseR);
		break;
	case(0x4000): // JSR
		if(numberData[4]) { // if 1, JSR with PCoffset
			if(numberData[5]) // if PCoffset is negative, sign extend negative sign
				PCoffset = instruction | 0xF000;
			else // else if positive, sign extend positive sign
				PCoffset = instruction & 0x07FF;

			if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
			// if symbols exist, see if one is given for our instruction and print that
				printf("JSR ");
				printf("%s\n", label);
			}
			// if no label, print out manual PCoffset

			else if(PCoffset < 0)
				printf("JSR PC%d\n", PCoffset);
			else if(PCoffset == 0)
				printf("JSR PC\n");
			else
				printf("JSR PC+%d\n", PCoffset);
		} else { // if 0, JSRR with register
			BaseR = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
			printf("JSRR R%d\n", BaseR);	
		}
		break;
	case(0xF000): // TRAP
		instruction &= 0x00FF; // get trap vector (8-bits) only
		switch(instruction) {
			case(0x0020):
				printf("GETC\n");
				break;
			case(0x0021):
				printf("OUT\n");
				break;
			case(0x0022):
				printf("PUTS\n");
				break;
			case(0x0023):
				printf("IN\n");
				break;
			case(0x0024):
				printf("PUTSP\n");
				break;
			case(0x0025):
				printf("HALT\n");
				break;
			default:
				printf("UNDEFINED TRAP\n");
				break;
		}		
		break;
	case(0x2000): // LD
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		if(numberData[7]) // if negative
			PCoffset = instruction | 0xFF00;
		else
			PCoffset = instruction & 0x00FF;

		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
		// if symbols exist, see if one is given for our instruction and print that
			printf("LD R%d, %s\n", DR, label);
		}
		// if no label, print out manual PCoffset
		else if(PCoffset < 0)
			printf("LD R%d, PC%d\n", DR, PCoffset);
		else if(PCoffset == 0)
			printf("LD R%d, PC\n", DR);
		else
			printf("LD R%d, PC+%d\n", DR, PCoffset);
		break;
	case(0xA000): // LDI
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		if(numberData[7]) // if negative
			PCoffset = instruction | 0xFF00;
		else
			PCoffset = instruction & 0x00FF;

		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
		// if symbols exist, see if one is given for our instruction and print that
			printf("LDI R%d, %s\n", DR, label);
		}
		// if no label, print out manual PCoffset


		else if(PCoffset < 0)
			printf("LDI R%d, PC%d\n", DR, PCoffset);
		else if(PCoffset == 0)
			printf("LDI R%d, PC\n", DR);
		else
			printf("LDI R%d, PC+%d\n", DR, PCoffset);
		break;
	case(0x6000): // LDR
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		BaseR = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
		if(numberData[10]) // negative check
			offset = instruction | 0xFFC0;
		else 
			offset = instruction & 0x003F;
		printf("LDR R%d, R%d, #%d\n", DR, BaseR, offset);
		break;
	case(0xE000): // LEA
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		if(numberData[7]) // if negative
			PCoffset = instruction | 0xFF00;
		else
			PCoffset = instruction & 0x00FF;

		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
			// if symbols exist, see if one is given for our instruction and print that
			printf("LEA R%d, %s\n", DR, label);
		}
		// if no label, print out manual PCoffset

		else if(PCoffset < 0)
			printf("LEA R%d, PC%d\n", DR, PCoffset);
		else if(PCoffset == 0)
			printf("LEA R%d, PC\n", DR);
		else
			printf("LEA R%d, PC+%d\n", DR, PCoffset);
		break;
	case(0x9000): // NOT
		DR = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		SR1 = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
		printf("NOT R%d, R%d\n", DR, SR1);
		break;
	case(0x3000): // ST
		SR1 = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		if(numberData[7]) // if negative
			PCoffset = instruction | 0xFF00;
		else
			PCoffset = instruction & 0x00FF;

		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
			// if symbols exist, see if one is given for our instruction and print that
			printf("ST R%d, %s\n", SR1, label);
		}
		// if no label, print out manual PCoffset

		else if(PCoffset < 0)
			printf("ST R%d, PC%d\n", SR1, PCoffset);
		else if(PCoffset == 0)
			printf("ST R%d, PC\n", SR1);
		else
			printf("ST R%d, PC+%d\n", SR1, PCoffset);
		break;
	case(0xB000): // STI
		SR1 = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		if(numberData[7]) // if negative
			PCoffset = instruction | 0xFF00;
		else
			PCoffset = instruction & 0x00FF;

		if((label = findLabel(binData[0] + i + PCoffset, symbols)) != NULL) {
			// if symbols exist, see if one is given for our instruction and print that
			printf("STI R%d, %s\n", SR1, label);
		}
		// if no label, print out manual PCoffset

		else if(PCoffset < 0)
			printf("STI R%d, PC%d\n", SR1, PCoffset);
		else if(PCoffset == 0)
			printf("STI R%d, PC\n", SR1);
		else
			printf("STI R%d, PC+%d\n", SR1, PCoffset);
		break;
	case(0x7000): // STR
		SR1 = 4 * numberData[4] + 2 * numberData[5] + numberData[6];
		BaseR = 4 * numberData[7] + 2 * numberData[8] + numberData[9];
		if(numberData[10]) // negative check
			offset = instruction | 0xFFC0;
		else 
			offset = instruction & 0x003F;
		printf("STR R%d, R%d, #%d\n", SR1, BaseR, offset);
		break;
	default:
		printf("NOP\n");
		break;
	}
	free(numberData);
	return;
}
