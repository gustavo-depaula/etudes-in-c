#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct  {
    void* pointer;
    size_t array_size, unit_size;
    // need the unit size to iterate over the array
    // size_t is a type guaranteed to hold any array index
} generic_array;

/* ============================================ */

void* get_unit(generic_array array,
               size_t i) {
    return &array.pointer[i * array.unit_size];
}

void print_array(generic_array array, void (*print)(void *)) {
    if (array.array_size == 0) {
        printf("empty array!\n");
    }

    size_t i;
    for (i = 0;
         i < (array.array_size-1);
         i = i + array.unit_size)
    {
        printf("[");
        print(get_unit(array, i));
        printf("] -> ");
    }

    printf("[");
    print(&array.pointer[i]);
    printf("]\n");
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

void write_to_file(char* file_name,
                   generic_array array,
                   char* (*to_string)(void *)) {
    FILE* output;
    output = fopen(file_name, "w");

    size_t i;
    for (i = 0;
         i < array.array_size * array.unit_size;
         i = i + array.unit_size)
    {
        char* string = to_string(&array.pointer[i]);
        fputs(string, output);
        free(string);
    }
    fclose(output);
}

/* ============================================ */

size_t get_parent_index(size_t i) {
    return i/2;
}
size_t get_left_child_index(size_t i) {
    return 2*i;
}
bool has_left_child(generic_array heap, size_t elem_index) {
    return get_left_child_index(elem_index) <= heap.array_size;
}
size_t get_right_child_index(size_t i) {
    return 2*i+1;
}
bool has_right_child(generic_array heap, size_t elem_index) {
    return get_right_child_index(elem_index) <= heap.array_size;
}

void* get_left_child(generic_array heap, size_t elem_index) {
    return get_unit(heap, get_left_child_index(elem_index));
}
void* get_right_child(generic_array heap, size_t elem_index) {
    return get_unit(heap, get_right_child_index(elem_index));
}

generic_array make_array_copy(generic_array source) {
    size_t array_size = source.array_size;
    size_t unit_size = source.unit_size;
    void* pointer = malloc(array_size * unit_size);
    generic_array copy = {
      .pointer = pointer,
      .array_size = array_size,
      .unit_size = unit_size
    };

    size_t i;
    for (i = 0;
         i < array_size * unit_size;
         i = i + unit_size)
    {
        void* elem = &source.pointer[i];
        memcpy(&pointer[i], elem, unit_size);
    }

    return copy;
}

void swap_array_positions(generic_array array,
                          size_t i,
                          size_t j) {
    void* aux = malloc(array.unit_size);
    memcpy(aux, &array.pointer[i], array.unit_size);
    memcpy(&array.pointer[i], &array.pointer[j], array.unit_size);
    memcpy(&array.pointer[j], aux, array.unit_size);
}

size_t get_top_child_index(generic_array heap, size_t index, bool (*comparator)(void *, void *)) {
    if (!has_left_child(heap, index)) {
        return 0;
    }

    void* left_child = get_left_child(heap, index);
    size_t top_child_index = get_left_child_index(index);

    if (has_right_child(heap, index)) {
        void* right_child = get_right_child(heap, index);
        if (comparator(left_child, right_child)) {
            top_child_index = get_right_child_index(index);
        }
    }

    return top_child_index;
}

void heapify(generic_array heap, size_t index, bool (*comparator)(void *, void *)) {
    size_t top_child_index = get_top_child_index(heap, index, comparator);
    if (top_child_index == 0) {
        return;
    }

    void* elem = get_unit(heap, index);
    void* top_child = get_unit(heap, top_child_index);
    if (comparator(elem, top_child)) {
        return;
    }

    swap_array_positions(heap, index, top_child_index);
    heapify(heap, top_child_index, comparator);
}

generic_array make_heap(generic_array from, bool (*comparator)(void *, void *)) {
    generic_array heap = make_array_copy(from);

    if (heap.array_size > 0) {
        size_t i = 0;
        for (i = heap.array_size; i >= 0; --i) {
            heapify(heap, i, comparator);
        }
    }

    return heap;
}

/* ============================================ */

void print_pointer(void* p) {
    printf("%p", p);
}

void print_int(void* i) {
    int int_i = *(int*)i;
    printf("%i", int_i);
}

bool compare_int(void* i, void* j) {
    int int_i = *(int*)i;
    int int_j = *(int*)j;
    return int_i < int_j;
}

/* ============================================ */

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

char* entity_to_string(void* e) {
    entity entity_e = *(entity*)e;

    char *str = malloc(256*sizeof(char));
    snprintf(str, 256, "%s %i\n", entity_e.url, entity_e.amount);
    return str;
}

void flush_entities_to_file(generic_array entities, size_t tape_id) {
    char tape_filename[32];
    snprintf(tape_filename, 32, "tapes/tape-%zu.txt", tape_id);
    generic_array merged = merge_sort(entities, compare_entity);
    write_to_file(tape_filename,
                  merged,
                  entity_to_string);
    free(merged.pointer);
}

void split_input_file_into_sorted_tapes(char* filename, size_t max_entities_per_tape) {
    FILE* input = fopen(filename, "r");

    entity entities_buffer[max_entities_per_tape];
    char line[256];
    size_t tape_id = 0;
    size_t line_number = 0;
    while (fgets(line, 256, input)) {
        if (line_number == max_entities_per_tape) {
            generic_array entities = {.pointer = &entities_buffer,
                                      .array_size=line_number,
                                      .unit_size=sizeof(entity)};
            flush_entities_to_file(entities, tape_id);
            ++tape_id;
            line_number = 0;
        }
        entities_buffer[line_number] = make_entity(line);
        ++line_number;
    }
    generic_array entities = {.pointer = &entities_buffer,
                              .array_size=line_number,
                              .unit_size=sizeof(entity)};
    flush_entities_to_file(entities, tape_id);

    fclose(input);
}

/* ============================================ */

int main() {
    split_input_file_into_sorted_tapes("./input.txt", 10);
    return 0;
}
