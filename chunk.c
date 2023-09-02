#include "include/chunk.h"

void init_chunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = nil;
    init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count+1) {
        const size_t old_capacity = chunk->capacity;
        chunk->capacity = grow_capacity(old_capacity);
        chunk->code = grow_array(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count++] = byte;
}

size_t add_constant(Chunk *chunk, Value value) {
    write_value_array(*chunk->constants, value);
    return chunk->constants.count-1;
}

void free_chunk(Chunk *chunk) {
    free_array(uint8_t, chunk->code, chunk->capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}