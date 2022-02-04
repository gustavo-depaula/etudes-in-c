#ifndef MERGE_SORT_H_
#define MERGE_SORT_H_

#include "generic_arrays.h"

typedef bool (*comparator_func)(void *, void *);

generic_array merge_sort(generic_array array,
                         comparator_func compartor);

#endif // MERGE_SORT_H_
