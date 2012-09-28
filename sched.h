#define MSP_SAVE(task) asm("                \
        str sp, [%[task_sp]]    \r\n        \
       "                                    \
       ::[task_sp] "l" (&task->stack_top)   \
       :"memory");

#define MSP_RESTORE(task) asm("             \
        ldr sp, [%[task_sp]]    \r\n        \
       "                                    \
       ::[task_sp] "l" (&task->stack_top)   \
       :"memory");

typedef struct partial_context {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
} partial_context;

typedef struct tcb {
    uint32_t* stack_top;
    uint32_t* stack_base;
    uint32_t* stack_limit;
    partial_context saved_partial_context;
} tcb;


void restore_partial_context(tcb* task);
void save_partial_context(tcb* task);
void os_tick(void) __attribute__((naked));
tcb* create_context(void(*func)(void));
