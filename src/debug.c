#include "include/debug.h"

void disassemble_chunk(Chunk *chunk, const char *name) {
    printf("--==-- %s --==--\n", name);

    for (size_t offset = 0; offset < chunk->count; /**/) {
        disassemble_instruction(chunk, offset);
    }
}

size_t disassemble_instruction(Chunk *chunk, size_t offset) {
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset-1]) {
        printf(" | ");
    } else {
        printf("%4d  ", chunk->lines[offset]);
    }

    const uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case op_constant:
            return constant_instruction(chunk, "op-constant", offset);
        case op_add:
            return simple_instruction("op-add", offset);
        case op_sub:
            return simple_instruction("op-sub", offset);
        case op_mul:
            return simple_instruction("op-mul", offset);
        case op_div:
            return simple_instruction("op-div", offset);
        case op_mod:
            simple_instruction("op-mod", offset);
        case op_negate:
            return simple_instruction("op-negate", offset);
        case op_return:
            return simple_instruction("op-return", offset);
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