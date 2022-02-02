#include <stdlib.h>
#include "merge_sort.h"

generic_array merge(generic_array array1,
                    generic_array array2,
                    comparator_func comparator) {
    size_t final_array_size = array1.array_size + array2.array_size;
    size_t unit_size = array1.unit_size;
    void* pointer = malloc(final_array_size * unit_size);
    generic_array final_array = {
      .pointer = pointer,
      .array_size = final_array_size,
      .unit_size = unit_size
    };

    size_t i;

    size_t array1_i = 0;
    size_t array2_i = 0;
    for (i = 0;
         i < final_array_size;
         ++i)
    {
        void* array1_head = get_unit(array1, array1_i);
        void* array2_head = get_unit(array2, array2_i);
        void* head = get_unit(final_array, i);

        if (array1_i == array1.array_size) {
            memcpy(head, array2_head, unit_size);
            ++array2_i;
            continue;
        }

        if (array2_i == array2.array_size) {
            memcpy(head, array1_head, unit_size);
            ++array1_i;
            continue;
        }

        if (comparator(array1_head, array2_head)) {
            memcpy(head, array1_head, unit_size);
            ++array1_i;
        } else {
            memcpy(head, array2_head, unit_size);
            ++array2_i;
        }
    }
    return final_array;
}

generic_array merge_sort(generic_array array,
                         comparator_func comparator) {
    if (array.array_size == 1) {
        return array;
    }

    int middle = array.array_size / 2;

    generic_array first_half = {
      .pointer = array.pointer,
      .array_size = middle,
      .unit_size = array.unit_size
    };

    generic_array last_half = {
      .pointer = array.pointer + array.unit_size*(middle),
      .array_size = array.array_size - middle,
      .unit_size = array.unit_size
    };
    generic_array sorted_first_half = merge_sort(first_half, comparator);
    generic_array sorted_last_half = merge_sort(last_half, comparator);

    return merge(sorted_first_half,
                 sorted_last_half,
                 comparator);
}
