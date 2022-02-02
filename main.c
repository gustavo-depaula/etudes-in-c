#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct  {
    void* pointer;
    size_t array_size, unit_size;
    // need the unit size to iterate over the array
} generic_array;

void print_pointer(void* p) {
    printf("%p", p);
}

void print_int(void* i) {
    int int_i = *(int*)i;
    printf("%i", int_i);
}

void print_array(generic_array array, void (*print)(void *)) {
    if (array.array_size == 0) {
        printf("empty array!\n");
    }

    size_t i;
    for (i = 0;
         i < (array.array_size-1) * array.unit_size;
         i = i + array.unit_size)
    {
        printf("[");
        print(&array.pointer[i]);
        printf("] -> ");
    }

    printf("[");
    print(&array.pointer[i]);
    printf("]\n");
}

bool compare_int(void* i, void* j) {
    int int_i = *(int*)i;
    int int_j = *(int*)j;
    return int_i < int_j;
}

void assign_int(void* dest, void* src) {
    *(int*)dest = *(int*)src;
}

generic_array merge(generic_array array1,
                    generic_array array2,
                    bool (*comparator)(void *, void *)) {
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
         i < final_array_size * unit_size;
         i = i + unit_size)
    {
        void* array1_head = &array1.pointer[array1_i];
        void* array2_head = &array2.pointer[array2_i];

        if (array1_i == (array1.array_size * array1.unit_size)) {
            memcpy((void*)(&pointer[i]), array2_head, unit_size);
            array2_i += unit_size;
            continue;
        }

        if (array2_i == (array2.array_size * array2.unit_size)) {
            memcpy((void*)(&pointer[i]), array1_head, unit_size);
            array1_i += unit_size;
            continue;
        }

        if (comparator(array1_head, array2_head)) {
            memcpy((void*)(&pointer[i]), array1_head, unit_size);
            array1_i += unit_size;
        } else {
            memcpy((void*)(&pointer[i]), array2_head, unit_size);
            array2_i += unit_size;
        }
    }
    return final_array;
}

generic_array merge_sort(generic_array array,
                         bool (*comparator)(void *, void *)) {
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

int callee(void* a, void* b) {
    int int_a = *(int*)a;
    int int_b = *(int*)b;
    printf("%i %i %i\n", int_a, int_b, int_a > int_b);
    return int_a > int_b;
}

int caller(void* a, void* b, int (*fun)(void*, void*)) {
    return fun(a, b);
}


int main() {


    int x;
    int y;
    int z;

    /* void* ptr; */
    /* int* ptr; */

    /* ptr = &x; */
    /* x = 5; */

    /* printf("%i\n", *ptr); */
    /* printf("%p %p %p\n", &x, &y, &z); */
    /* printf("%zu\n", sizeof(int)); */
    /* printf("%p %p %p %p\n", &x, &y, &z, ptr); */
    /* printf("%i\n", *(int*)ptr); */
    /* printf("%p %p\n", &caller, &callee); */
    /* printf("%p %p\n", caller, callee); */


    /* int* a = &(int){4}; */
    /* int* b = &(int){3}; */
    /* printf("%i\n", caller(a, b, callee)); */


    /* a[1] => *(a+1); */
    /* 1[a] => *(1+a); */

    int a[] = {5,2,7,1,4,0};
    generic_array a_a = {.pointer = &a, .array_size = 6, .unit_size = sizeof(int)};

    /* int b[] = {2,4,6,8,10}; */
    /* generic_array a_b = {.pointer = &b, .array_size = 5, .unit_size = sizeof(int)}; */
    /* printf("%i", *(a+1)); */
    /* printf("%i", 1[a]); */


    /* printf("%p\n", a); */
    /* printf("%p\n", &a[0]); */
    /* printf("%p\n", &a[1]); */
    /* printf("%p\n", &a[1]); */


    /* printf("%i\n", a[0]); */
    /* void* ptr = &a[1]; */
    /* printf("%p\n", ptr); */

    /* int int_a = *(int*)ptr; */
    /* printf("%i\n\n\n\n", int_a); */
    /* printf("%zu\n\n\n", sizeof(int)); */

    /* print_array(a_a, print_int); */


    /* print_array(a_a, print_pointer); */
    /* print_array(a_b, print_pointer); */

    /* void** pointer = malloc(5*sizeof(int)); */
    /* void* ptr = &a[0]; */
    /* void** ptr2 = malloc(sizeof(int)); */
    /* ptr2[0] = &ptr; */
    /* printf("%i\n", &ptr); */
    /* printf("%i\n", *(int*)ptr); */
    /* printf("%p\n", ptr); */
    /* printf("%p\n", ptr2); */
    /* printf("%i\n", *(int*)ptr2); */
    /* memcpy(pointer[0], ptr, sizeof(int)); */

    /* generic_array merged = merge(a_a, a_b, compare_int); */
    /* print_array(merged, print_int); */
    /* print_array(merged, print_pointer); */

    generic_array merged = merge_sort(a_a, compare_int);
    print_array(merged, print_int);
    print_array(merged, print_pointer);
    return 0;
}
