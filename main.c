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

int main() {
    split_input_file_into_sorted_tapes("./input.txt", 10);
    return 0;
}
