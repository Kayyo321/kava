#include "include/value.h"

void init_value_array(ValueArray *array) {
    array->capacity = 0;
    array->count = 0;
    array->values = nil;
}

void write_value_array(ValueArray *array, Value value) {
    if (array->capacity < array->count + 1) {
        size_t old_capacity = array->capacity;
        array->capcity = grow_capacity(Value, array->values, old_capacity, array->capcity);
    }

    array->values[array->count++] = value;
}

void free_value_array(ValueArray *array) {
    free_array(Value, array->values, array->capacity);
    init_value_array(array);
}

void print_value(Value value) {
    printf("%g", value);
}