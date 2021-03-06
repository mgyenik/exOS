#define MSP_SAVE(ctask) asm("                       \
        str sp, [%[task_sp]]    \r\n                \
       "                                            \
       ::[task_sp] "l" (&ctask->task->stack_top)    \
       :"memory");

#define MSP_RESTORE(ctask) asm("                    \
        ldr sp, [%[task_sp]]    \r\n                \
       "                                            \
       ::[task_sp] "l" (&ctask->task->stack_top)    \
       :"memory");

#define EXCEPT_RETURN() asm("               \
        ldr lr, =#0xfffffff9    \r\n        \
        bx  lr                  \r\n        \
       "                                    \
       ::                                   \
       :"memory");

#define RESTORE_PARTIAL_CONTEXT(task) asm("                                     \
        ldmia   %[task_addr], {r4-r11}                          \r\n            \
        "                                                                       \
        ::[task_addr] "l" (&(task->saved_partial_context))                      \
        :"r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "memory"             \
        );

#define SAVE_PARTIAL_CONTEXT(task) asm("                                        \
        stmia   %[task_addr], {r4-r11}                          \r\n            \
        "                                                                       \
        ::[task_addr] "l" (&(task->saved_partial_context))                      \
        :"memory"                                                               \
        );

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
    uint32_t period;
} tcb;

typedef struct tcb_node {
    struct tcb_node* next;
    tcb* task;
} tcb_node;

extern tcb_node* curr_task;

void restore_partial_context(tcb* task);
void save_partial_context(tcb* task);
void os_tick(void) __attribute__((naked));
tcb* create_task(void(*func)(void));
tcb_node* create_task_node(void(*func)(void), uint32_t jiffies);
void task_insert(tcb_node* head, tcb_node* new);
void task_create(void(*func)(void), uint32_t jiffies);

void schedule(void);
