void init_region_as_dmem(uint32_t* start, uint32_t* end);
void* alloc(uint32_t size);

typedef struct mem_region {
    uint32_t* head;
    uint32_t* tail;
} mem_region;
