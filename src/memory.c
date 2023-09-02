#include "include/memory.h"

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return nil;
    }

    void *result = realloc(pointer, new_size);
    if (result == nil) {
        exit(EXIT_FAILURE);
    }

    return result;
}