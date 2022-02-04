#include "generic_arrays.h"
#include "heaps.h"


size_t get_parent_index(size_t i) {
    return (i-1)/2;
}
size_t get_left_child_index(size_t i) {
    return 2*i+1;
}
bool has_left_child(generic_array heap, size_t elem_index) {
    return get_left_child_index(elem_index) < heap.array_size;
}
size_t get_right_child_index(size_t i) {
    return 2*i+2;
}
bool has_right_child(generic_array heap, size_t elem_index) {
    return get_right_child_index(elem_index) < heap.array_size;
}

void* get_parent(generic_array heap, size_t elem_index) {
    return get_unit(heap, get_parent_index(elem_index));
}
void* get_left_child(generic_array heap, size_t elem_index) {
    return get_unit(heap, get_left_child_index(elem_index));
}
void* get_right_child(generic_array heap, size_t elem_index) {
    return get_unit(heap, get_right_child_index(elem_index));
}

void print_heap(generic_array heap, size_t index, void (*print)(void *)) {
    if (index >= heap.array_size) {
        return;
    }

    printf("[");
    print(get_unit(heap, index));
    printf("] -> [");
    if (has_left_child(heap, index)) {
        print(get_left_child(heap, index));
        printf(", ");
        if (has_right_child(heap, index)) {
            print(get_right_child(heap, index));
        } else {
            printf("(empty)");
        }
    } else {
        printf("no children!");
    }
    printf("]\n");

    print_heap(heap, get_left_child_index(index), print);
    print_heap(heap, get_right_child_index(index), print);
}



size_t get_top_child_index(generic_array heap, size_t index, bool (*comparator)(void *, void *)) {
    if (!has_left_child(heap, index)) {
        return 0;
    }

    void* left_child = get_left_child(heap, index);
    size_t top_child_index = get_left_child_index(index);

    if (has_right_child(heap, index)) {
        void* right_child = get_right_child(heap, index);
        if (comparator(right_child, left_child)) {
            top_child_index = get_right_child_index(index);
        }
    }

    return top_child_index;
}

void heapify_bottom_down(generic_array heap, size_t index, bool (*comparator)(void *, void *)) {
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
    heapify_bottom_down(heap, top_child_index, comparator);
}

void heapify_bottom_up(Heap heap, size_t index) {
    if (index == 0) {
        return;
    }

    void* elem = get_unit(heap.array, index);
    void* parent = get_parent(heap.array, index);
    if (heap.comparator(parent, elem)) {
        return;
    }

    swap_array_positions(heap.array, index, get_parent_index(index));
    heapify_bottom_up(heap, get_parent_index(index));
}

Heap make_heap(generic_array from, bool (*comparator)(void *, void *)) {
    generic_array heap = make_array_copy(from);

    if (heap.array_size > 0) {
        size_t i;
        for (i = heap.array_size-1; i > 0; --i) {
            heapify_bottom_down(heap, i, comparator);
        }
        heapify_bottom_down(heap, 0, comparator);
    }

    Heap* as_heap = (Heap*)&heap;
    as_heap->comparator = comparator;

    return *as_heap;
}

void* extract(Heap* heap) {
    generic_array heap_array = *(generic_array*)heap;
    if (heap_array.array_size == 0) {
        return NULL;
    }
    void* extracted = get_unit_copy(heap_array, 0);

    void* rightMostElement = get_unit(heap_array, heap_array.array_size-1);
    assign_unit_to_position(heap_array, 0, rightMostElement);
    heapify_bottom_down(heap_array, 0, heap->comparator);

    decrease_array_size((generic_array*)heap);

    return extracted;
}

void insert(Heap* heap, void* unit) {
    increase_array_size(&heap->array);

    size_t lastPosition = (*heap).array.array_size-1;
    assign_unit_to_position(heap->array, lastPosition, unit);
    heapify_bottom_up(*heap, lastPosition);
}
