#ifndef KAVA_VALUE_H
#define KAVA_VALUE_H

#include "common.h"
#include "memory.h"

typedef enum {
    val_boolean,
    val_nothing,
    val_number
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define is_boolean(value) ((value).type == val_boolean)

#define is_nothing(value) ((value).type == val_nothing)

#define is_number(value) ((value).type == val_number)

#define as_boolean(value) ((value).as.boolean)

#define as_number(value) ((value).as.number)

#define make_boolean(value)\
((Value){val_boolean, {.boolean=value}})

#define make_nothing\
((Value){val_nothing, {.number = 0.0}})

#define make_number(value)\
((Value){val_number}, {.number=value})

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