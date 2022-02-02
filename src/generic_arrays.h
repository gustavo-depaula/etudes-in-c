#ifndef GENERIC_ARRAYS_H_
#define GENERIC_ARRAYS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Here's how to create a generic_array from an int array:

int array[] = {5,2,7,1,4,0};
generic_array g_array = {.pointer = &array, .array_size = 6, .unit_size = sizeof(int)};

 */
typedef struct  {
    void* pointer;
    size_t array_size, unit_size;
    // need the unit size to iterate over the array
    // size_t is a type guaranteed to hold any array index
} generic_array;
typedef void (*printer_func)(void *);

void* get_unit(generic_array array, size_t i);
void* get_unit_copy(generic_array array, size_t i);
void print_array(generic_array array, printer_func);
void assign_unit_to_position(generic_array array, size_t index, void* unit);
void swap_array_positions(generic_array array, size_t i,size_t j);
void change_array_size(generic_array* array, size_t new_size);
void increase_array_size(generic_array* array);
void decrease_array_size(generic_array* array);

#endif // GENERIC_ARRAYS_H_
