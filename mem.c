#include <registers.h>
#include <mem.h>


mem_region simple_dmem = {.head = NULL,
                    .tail = NULL};

void init_region_as_dmem(uint32_t* start, uint32_t* end) {
    simple_dmem.head = start;
    simple_dmem.tail = end;
    while(start < end) {
        *start = (uint32_t)start + 4;
        start++;
    }
}

void* alloc(uint32_t size) {
    size += size%4;
    if(simple_dmem.head == NULL || simple_dmem.tail == NULL) {
        return NULL;
    }
    if(simple_dmem.head < simple_dmem.tail) {
        void* ret = simple_dmem.head;
        for(int i = size/4; i; i--) {
            simple_dmem.head += 4;
        }
        return ret;
    }
    else {
        return NULL;
    }
}
