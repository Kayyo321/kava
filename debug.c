#include "include/debug.h"

void disassemble_chunk(Chunk *chunk, const char *name) {
    printf("--==-- %s --==--\n", name);

    for (size_t offset = 0; offset < chunk->count; /**/) {
        disassemble_instruction(chunk, offset);
    }
}

size_t disassemble_instruction(Chunk *chunk, size_t offset) {
    printf("%04d ", offset);

    const uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case op_return:
            return simple_instruction("op-return", offset);
        case op_constant:
            return constant_instruction(chunk, "op-constant", offset);
        default:
            printf("unknown op-code %d\n", instruction);
            return offset+1;
    }
}

static size_t simple_instruction(const char *name, size_t offset) {
    printf("%s\n", name);
    return offset+1;
}

static size_t constant_instruction(Chunk *chunk, const char *name, size_t offset) {
    const uint8_t constant = chunk->code[offset+1];

    printf("%-16s  %4d  '", name, constant);

    print_value(chunk->constants.values[constant]);
    printf("'\n");

    return offset+2;
}