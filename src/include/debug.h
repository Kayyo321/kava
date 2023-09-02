#ifndef KAVA_DEBUG_H
#define KAVA_DEBUG_H

#include "common.h"
#include "chunk.h"
#include "value.h"

void disassemble_chunk(Chunk *, const char *);
size_t disassemble_instruction(Chunk *, size_t);
static size_t simple_instruction(const char *, size_t);
static size_t constant_instruction(Chunk *, const char *, size_t);

#endif