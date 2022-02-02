#ifndef PRINTERS_H_
#define PRINTERS_H_

#include "stdio.h"

void print_pointer(void* p) {
    printf("%p", p);
}

void print_int(void* i) {
    int int_i = *(int*)i;
    printf("%i", int_i);
}

#endif // PRINTERS_H_
