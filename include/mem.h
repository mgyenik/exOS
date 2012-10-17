void init_region_as_dmem(uint32_t* start, uint32_t* end);
void* alloc(uint32_t size);
void free(void* mem);

typedef struct mem_region {
    uint32_t* head;
    uint32_t* tail;
} mem_region;

/*
typedef struct bblock {
    uint8_t size;
    uint8_t free;
    uint16_t next_offset;
} bblock;

typedef struct bheap_list {
    bblock* head;
    bblock* tail;
} bheap_list;

typedef struct bheap {
    semaphore* heap_lock;
    bheap_list freelists[HEAP_MAXP + 1];
} bheap;
*/
