#ifndef KAVA_MEMORY_H
#define KAVA_MEMORY_H

#include "common.h"

#define grow_capacity(capacity)\
((capacity) < 8 ? 8 : (capacity) * 2)

#define grow_array(type, pointer, old_count, new_count)\
(type*)reallocate(pointer, sizeof(type) * old_count, sizeof(type) * new_count)

void *reallocate(void *, size_t, size_t);

#endif