#ifndef EXTERNAL_SORTING_H_
#define EXTERNAL_SORTING_H_

#include <stdlib.h>
#include <stdbool.h>
#include "merge_sort.h"


void external_sorting(char* input_filename,
                      char* output_filename,
                      size_t max_lines_per_tape,
                      comparator_func comparator,
                      char* tape_filename_format);

#endif // EXTERNAL_SORTING_H_
