#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "generic_arrays.h"
#include "printers.h"
#include "heaps.h"
#include "merge_sort.h"
#include "external_sorting.h"

char* get_tape_filename(size_t tape_id, char* tape_filename_format) {
    char* tape_filename = malloc(128*sizeof(char));
    snprintf(tape_filename, 128, tape_filename_format, tape_id);
    return tape_filename;
}

char* void_pointer_to_string(void* p) {
    return strdup(p);
}

void flush_lines_to_tape(char** lines,
                         size_t number_of_lines,
                         size_t tape_id,
                         comparator_func comparator,
                         char* tape_filename_format) {
    generic_array units = {.pointer = lines[0],
                            .array_size=number_of_lines,
                            .unit_size=sizeof(char)*256};
    generic_array merged = merge_sort(units, comparator);

    write_array_to_file(get_tape_filename(tape_id, tape_filename_format),
                        merged,
                        void_pointer_to_string);
}

size_t split_input_file_into_sorted_tapes(char* input_filename,
                                          size_t max_lines_per_tape,
                                          comparator_func comparator,
                                          char* tape_filename_format) {
    FILE* input = fopen(input_filename, "r");
    char line[256];
    char* lines_buffer[max_lines_per_tape];

    size_t i;
    for (i = 0; i < max_lines_per_tape; i++) {
        lines_buffer[i] = malloc(256 * sizeof(char));
    }

    size_t tape_id = 0;
    size_t line_number = 0;
    while (fgets(line, 256, input)) {
        if (line_number == max_lines_per_tape) {
            flush_lines_to_tape(lines_buffer,
                                line_number,
                                tape_id,
                                comparator,
                                tape_filename_format);
            ++tape_id;
            line_number = 0;
        }
        strcpy(lines_buffer[line_number], line);
        ++line_number;
    }

    flush_lines_to_tape(lines_buffer,
                        line_number,
                        tape_id,
                        comparator,
                        tape_filename_format);

    for (i = 0; i < max_lines_per_tape; i++) {
        free(lines_buffer[i]);
    }

    return tape_id+1;
}

typedef struct {
    size_t tape_id;
    char* line;
} data;


void print_data(void* d) {
    data data_d = *(data*)d;
    printf("tape_id=%zu | line=%s", data_d.tape_id, data_d.line);
}

comparator_func GLOBAL_COMPARATOR;

void set_global_comparator(comparator_func comparator) {
    GLOBAL_COMPARATOR = comparator;
}

bool compare_data(void* d1, void* d2) {
    data data_d1 = *(data*)d1;
    data data_d2 = *(data*)d2;
    return GLOBAL_COMPARATOR(data_d1.line, data_d2.line);
}


void merge_tapes_into_output_file(size_t number_of_tapes,
                                  char* output_filename,
                                  comparator_func comparator,
                                  char* tape_filename_format) {
    FILE* tapes[number_of_tapes];
    FILE* output = fopen(output_filename, "w");

    size_t i;
    for (i = 0; i < number_of_tapes; ++i) {
        char* tape_filename = get_tape_filename(i, tape_filename_format);
        tapes[i] = fopen(tape_filename, "r");
        free(tape_filename);
    }

    char line[256];
    data top_data[number_of_tapes];
    for (i = 0; i < number_of_tapes; ++i) {
        fgets(line, 256, tapes[i]);
        top_data[i] = (data){.tape_id = i, .line = strdup(line)};
    }

    generic_array datas = {.pointer = &top_data,
                           .array_size=number_of_tapes,
                           .unit_size=sizeof(data)};

    set_global_comparator(comparator);
    Heap heap = make_heap(datas, compare_data);

    data* extracted = extract(&heap);
    while (extracted != NULL) {
        fputs(extracted->line, output);

        if (fgets(line, 256, tapes[extracted->tape_id]) != NULL) {
            data d = { .line = strdup(line), .tape_id = extracted->tape_id };
            insert(&heap, &d);
        }
        extracted = extract(&heap);
    }

    // close files
    for (i = 0; i < number_of_tapes; ++i) {
        fclose(tapes[i]);
    }
    fclose(output);
}

void external_sorting(char* input_filename,
                      char* output_filename,
                      size_t max_lines_per_tape,
                      comparator_func comparator,
                      char* tape_filename_format) {

    size_t number_of_tapes = split_input_file_into_sorted_tapes(input_filename,
                                                                max_lines_per_tape,
                                                                comparator,
                                                                tape_filename_format);
    merge_tapes_into_output_file(number_of_tapes,
                                 output_filename,
                                 comparator,
                                 tape_filename_format);
}
