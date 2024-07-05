// Decoding binary data as `asm`
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define MOV_OP 136

void print_bin(uint16_t);

const char *reg[] = {
	"al",
	"cl",
	"dl",
	"bl",
	"ah",
	"ch",
	"dh",
	"bh",
	"ax",
	"cx",
	"dx",
	"bx",
	"sp",
	"bp",
	"si",
	"di"
};

const char *get_reg(uint8_t *instr) {
	uint8_t idx = 1 & instr[0];
	if (idx) idx = idx << 3;
	idx = idx | (((7 << 3) & instr[1]) >> 3);
	return reg[idx];
}

const char *get_rm(uint8_t *instr) {
	uint8_t idx = 1 & instr[0];
	if (idx) idx = idx << 3;
	idx = idx | (7 & instr[1]);
	return reg[idx];
}

bool ismovop(uint8_t *instr) {

	uint16_t mask = (~0) << 2;
	return (*instr & mask) == MOV_OP;
}

void print_bin(uint16_t buf) {
	size_t pos = 1;
	pos = pos << ((sizeof(buf) * 8) - 1);
	while (pos > 0) {
		if (buf & pos) {
			printf("1");
		} else {
			printf("0");
		}
		pos = pos >> 1;
	}
	printf("\n");
}

void print_movop(const char *val[]) {
}

void print_listing_37() {
	FILE *file= fopen("listing_0037_single_register_mov", "rb");
	uint8_t *instr = malloc(sizeof(uint8_t) * 2);
	fread(instr, 1, 2, file);
	assert(ismovop(instr));
	const char *reg = get_reg(instr);
	const char *rm = get_rm(instr);

	const char *operands[2];
	if (2 & instr[0]) {
		operands[0] = reg;
		operands[1] = rm;
	} else {
		operands[1] = reg;
		operands[0] = rm;
	}

	printf("mov %s, %s\n\n", operands[0], operands[1]);
	free(instr);
	fclose(file);
}

void print_listing_38() {
	FILE *file= fopen("listing_0038_many_register_mov", "rb");
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	uint8_t *asm_data = malloc(sizeof(uint8_t) * size);
	fseek(file, 0, SEEK_SET);
	fread(asm_data, size, 1, file);
	for (int i = 0; i < (size >> 1); i++) {
		uint8_t *instr = asm_data + i * 2;
		assert(ismovop(instr));
		const char *reg = get_reg(instr);
		const char *rm = get_rm(instr);

		const char *operands[2];
		if (2 & instr[0]) {
			operands[0] = reg;
			operands[1] = rm;
		} else {
			operands[1] = reg;
			operands[0] = rm;
		}

		printf("mov %s, %s\n", operands[0], operands[1]);
	}

	free(asm_data);
	fclose(file);
}

int main(void) {
	printf("\n[Listing 37]\n");
	print_listing_37();
	printf("\n[Listing 37]\n");
	print_listing_38();

	return 0;
}

// input-> 10001000 00000000
// int  -> 8 + 128  
