#ifndef _fat_pointers_h
#define _fat_pointers_h

#include <stdlib.h>
struct Header {
    int type;
};

void* alloc_array(size_t data_size, int type);

void dealloc_array(void* self);

void* realloc_array(void* data, size_t data_size, int type);

int get_array_length(void* data); 

#endif
