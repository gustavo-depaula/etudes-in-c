#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "generic_arrays.h"
#include "merge_sort.h"
#include "heaps.h"


/* ============================================ */

typedef struct  {
    char *url;
    int amount;
    int tape_id;
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

char* get_tape_filename(size_t tape_id) {
    char* tape_filename = malloc(32*sizeof(char));
    snprintf(tape_filename, 32, "tapes/tape-%zu.txt", tape_id);
    return tape_filename;
}

void flush_entities_to_file(generic_array entities, size_t tape_id) {
    generic_array merged = merge_sort(entities, compare_entity);
    char* tape_filename = get_tape_filename(tape_id);
    write_array_to_file(tape_filename,
                  merged,
                  entity_to_string);
    free(tape_filename);
    free(merged.pointer);
}

size_t split_input_file_into_sorted_tapes(char* filename, size_t max_entities_per_tape) {
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
    return tape_id+1;
}

void merge_tapes_into_output_file(size_t number_of_tapes) {
    FILE* tapes[number_of_tapes];
    FILE* output = fopen("output.txt", "w");

    size_t i;
    for (i = 0; i < number_of_tapes; ++i) {
        char* tape_filename = get_tape_filename(i);
        tapes[i] = fopen(tape_filename, "r");
        free(tape_filename);
    }

    char line[256];
    entity top_entities[number_of_tapes];
    for (i = 0; i < number_of_tapes; ++i) {
        fgets(line, 256, tapes[i]);
        top_entities[i] = make_entity(line);
        top_entities[i].tape_id = i;
    }

    generic_array entities = {.pointer = &top_entities,
                              .array_size=number_of_tapes,
                              .unit_size=sizeof(entity)};

    Heap heap = make_heap(entities, compare_entity);
    printf("\n\n");
    print_heap(heap.array, 0, print_entity);

    entity* extracted = extract(&heap);
    printf("entities:\n\n");
    while (extracted != NULL) {
        fputs(entity_to_string(extracted), output);

        if (fgets(line, 256, tapes[extracted->tape_id]) != NULL) {
            entity e = make_entity(line);
            e.tape_id = extracted->tape_id;
            insert(&heap, &e);
        }
        extracted = extract(&heap);
    }

    // close files
    for (i = 0; i < number_of_tapes; ++i) {
        fclose(tapes[i]);
    }
    fclose(output);
}

/* ============================================ */

int main() {
    size_t number_of_tapes = split_input_file_into_sorted_tapes("./input.txt", 10);
    merge_tapes_into_output_file(number_of_tapes);

    return 0;
}
