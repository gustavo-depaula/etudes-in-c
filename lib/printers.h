#ifndef PRINTERS_H_
#define PRINTERS_H_

#include "stdio.h"
#include <string.h>

void print_pointer(void* p) {
    printf("%p", p);
}

void print_int(void* i) {
    int int_i = *(int*)i;
    printf("%i", int_i);
}

void print_str(void* s) {
    char* str = strdup(s);
    printf("%s", str);
}

#endif // PRINTERS_H_
