#ifndef KAVA_CHUNK_H
#define KAVA_CHUNK_H

#include "common.h"
#include "memory.h"
#include "value.h"

#define new_chunk(c) nil; init_chunk(&c) 

typedef enum {
    op_constant,
    op_nothing,
    op_true,
    op_false,
    op_equal,
    op_greater,
    op_less,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,
    op_not,
    op_negate,
    op_return,
} OpCode;

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t *code;
    int *lines;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk *);
void write_chunk(Chunk *, uint8_t, int);
size_t add_constant(Chunk *, Value);
void free_chunk(Chunk *);

#endif