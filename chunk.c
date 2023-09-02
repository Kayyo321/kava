#include "include/chunk.h"

void init_chunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = nil;
}

void write_chunk(Chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count+1) {
        const size_t old_capacity = chunk->capacity;
        chunk->capacity = grow_capacity(old_capacity);
        chunk->code = grow_array(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count++] = byte;
}