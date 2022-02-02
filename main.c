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

typedef struct  {
    char *url;
    int amount;
    int tapeIdentifier;
} entity;

bool compare_entity(void* a, void* b) {
    entity entity_a = *(entity*)a;
    entity entity_b = *(entity*)b;
    if (entity_a.amount == entity_b.amount) {
        return strcmp(entity_a.url, entity_b.url) < 0;
    }
    return entity_a.amount > entity_b.amount;
}

void print_entity(void* a) {
    entity entity_a = *(entity*)a;
    printf("url=%s | amount=%i", entity_a.url, entity_a.amount);
}

entity make_entity(char* line) {
    char* url = strtok(strdup(line), " ");
    char* amount_str = strtok(NULL, " ");

    entity e = {.url = url, .amount = atoi(amount_str)};
    return e;
}

int main() {
    entity a = make_entity("google.com 59");
    print_entity((void*)&a);
    /* entity b = {.amount = 3, .url = "aaazon.com"}; */
    /* entity x = {.amount = 3, .url = "amazon.com"}; */
    /* entity a = {.amount = 3, .url = "google.com"}; */
    /* entity c = {.amount = 1, .url = "bing.com"}; */
    /* entity d = {.amount = 4, .url = "msft.com"}; */
    /* entity entities[] = {a,b,c,d,x}; */
    /* generic_array a_e = {.pointer = &entities, .array_size=5, .unit_size = sizeof(entity)}; */
    /* print_array(a_e, print_entity); */


    /* int a[] = {5,2,7,1,4,0}; */
    /* generic_array a_a = {.pointer = &a, .array_size = 6, .unit_size = sizeof(int)}; */

    /* int b[] = {2,4,6,8,10}; */
    /* generic_array a_b = {.pointer = &b, .array_size = 5, .unit_size = sizeof(int)}; */

    /* generic_array merged = merge_sort(a_e, compare_entity); */
    /* print_array(merged, print_entity); */
    /* print_array(merged, print_pointer); */
    return 0;
}
