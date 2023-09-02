#ifndef KAVA_CHUNK_H
#define KAVA_CHUNK_H

#include "common.h"
#include "memory.h"

typedef enum {
    op_return,
} OpCode;

typedef struct {
    size_t count;
    size_t capacity;
    uint8_t *code;
} Chunk;

void init_chunk(Chunk *);
void write_chunk(Chunk *, uint8_t);

#endif