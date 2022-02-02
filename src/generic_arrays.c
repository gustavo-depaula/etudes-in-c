#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "generic_arrays.h"

void* get_unit(generic_array array,
               size_t i) {
    return &array.pointer[i * array.unit_size];
}
void* get_unit_copy(generic_array array,
                    size_t i) {
    void* unit = get_unit(array, i);
    void* copy = malloc(array.unit_size);
    memcpy(copy, unit, array.unit_size);
    return copy;
}

void print_array(generic_array array, printer_func print) {
    if (array.array_size == 0) {
        printf("empty array!\n");
    }

    size_t i;
    for (i = 0;
         i < (array.array_size-1);
         ++i)
    {
        printf("[");
        print(get_unit(array, i));
        printf("] -> ");
    }

    printf("[");
    print(get_unit(array, i));
    printf("]\n");
}

void assign_unit_to_position(generic_array array,
                             size_t index,
                             void* unit) {
    memcpy(get_unit(array, index), unit, array.unit_size);
}

void swap_array_positions(generic_array array,
                          size_t i,
                          size_t j) {
    void* aux = malloc(array.unit_size);
    memcpy(aux, get_unit(array, i), array.unit_size);
    assign_unit_to_position(array, i, get_unit(array, j));
    assign_unit_to_position(array, j, aux);
    free(aux);
}

void change_array_size(generic_array* array, size_t new_size) {
    array->array_size = new_size;
    array->pointer = realloc(array->pointer, new_size * array->unit_size);
}

void increase_array_size(generic_array* array) {
    size_t new_array_size = array->array_size+1;
    change_array_size(array, new_array_size);
}
void decrease_array_size(generic_array* array) {
    size_t new_array_size = array->array_size-1;
    change_array_size(array, new_array_size);
}
