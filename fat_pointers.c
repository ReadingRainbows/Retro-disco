#include "fat_pointers.h"
#include <stdio.h>
#include <stdlib.h>

void* alloc_array(size_t data_size, int type) {
    struct Header* head = calloc(1, sizeof(struct Header) + data_size);

    head->type = type;

    return((void*)head) + sizeof(struct Header);
}

void* realloc_array(void* data, size_t data_size, int type) {
    struct Header* realloc_data = data - sizeof(struct Header);
    struct Header* head = realloc(realloc_data, sizeof(struct Header) + data_size);

    head->type = type;

    return((void*)head) + sizeof(struct Header);
}

void dealloc_array(void* self) {
    struct Header* head = self - sizeof(struct Header);
    free(head);
}

int get_array_length(void* data) {
    struct Header* array_length = data - sizeof(struct Header);
    return array_length->type;
}
