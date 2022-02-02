#ifndef HEAPS_H_
#define IFNDEF HEAPS_H_

#include "generic_arrays.h"

typedef bool (*comparator_func)(void *, void *);
typedef struct {
    generic_array array;
    comparator_func comparator;
} Heap;

Heap make_heap(generic_array from, bool (*comparator)(void *, void *));
void* extract(Heap* heap);
void insert(Heap* heap, void* unit);
void print_heap(generic_array heap, size_t index, void (*print)(void *));

#endif // IFNDEF HEAPS_H_
