#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../lib/generic_arrays.h"
#include "../../lib/merge_sort.h"
#include "../../lib/heaps.h"
#include "../../lib/external_sorting.h"


/* ============================================ */

typedef struct  {
    char *url;
    int amount;
} entity;

entity make_entity(char* line) {
    char* url = strtok(strdup(line), " ");
    char* amount_str = strtok(NULL, " ");

    entity e = {.url = url, .amount = atoi(amount_str)};
    return e;
}

bool compare_entities(void* a, void* b) {
    entity entity_a = make_entity(a);
    entity entity_b = make_entity(b);

    if (entity_a.amount == entity_b.amount) {
        return strcmp(entity_a.url, entity_b.url) < 0;
    }
    return entity_a.amount > entity_b.amount;
}

int main() {
    external_sorting("./cases/external-sort-urls/input.txt",
                     "./cases/external-sort-urls/output.txt",
                     10,
                     compare_entities,
                     "./cases/external-sort-urls/tapes/tape-%zu.txt");

    return 0;
}
