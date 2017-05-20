#include "fat_pointers.h"
#include <stdio.h>
#include <stdlib.h>

void* alloc_array(size_t data_size, int type) {
    struct Header* head = calloc(1, sizeof(struct Header) + data_size);

    head->type = type;

    return((void*)head) + sizeof(struct Header);
}

void dealloc_array(void* self) {
    struct Header* head = self - sizeof(struct Header);
    free(head);
}
