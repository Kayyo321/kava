#ifndef KAVA_VALUE_H
#define KAVA_VALUE_H

#include "common.h"
#include "memory.h"

typedef double Value;

typedef struct {
    size_t capacity;
    size_t count;
    Value *values;
} ValueArray;

void init_value_array(ValueArray *);
void write_value_array(ValueArray *, Value);
void free_value_array(ValueArray *);
void print_value(Value);

#endif