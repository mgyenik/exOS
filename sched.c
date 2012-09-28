#include "registers.h"
#include "sched.h"
#include "mem.h"
#include "systick.h"
#include "main.h"

extern uint32_t _sheap;
extern uint32_t _eheap;

#define STACK_SIZE 100
/*
tcb current_task = {
    .stack_top = (uint32_t*)0x10101010,
    .stack_base = (uint32_t*)0xd00dd00d,
    .stack_limit = (uint32_t*)0xd00dd00d,
    .saved_partial_context = {
        .r4 = 0xd00dd004,
        .r5 = 0xd00dd005,
        .r6 = 0xd00dd006,
        .r7 = 0xd00dd007,
        .r8 = 0xd00dd008,
        .r9 = 0xd00dd009,
        .r10 = 0xd00dd010,
        .r11 = 0xd00dd011}
};
*/

tcb* curr_task = NULL;

void restore_partial_context(tcb* task) {
    asm volatile("\
            add     %[task_addr], %[task_addr], %[context_offset] \r\n\
            ldmia   %[task_addr], {r4,r5,r6,r7,r8,r9,r10,r11}     \r\n\
        "
        :[task_addr] "+l" (task)
        :[context_offset] "I" (__builtin_offsetof(tcb, saved_partial_context))
        :"memory"
    );
} 

void save_partial_context(tcb* task) {
    asm volatile("\
            add     %[task_addr], %[task_addr], %[context_offset] \r\n\
            stmia   %[task_addr], {r4,r5,r6,r7,r8,r9,r10,r11} \r\n\
        "
        :[task_addr] "+l" (task)
        :[context_offset] "I" (__builtin_offsetof(tcb, saved_partial_context))
        :"memory"
    );
}

void os_start(void) {
    init_region_as_dmem(&_sheap, &_eheap);
    curr_task = create_context(main);
    init_systick();
    
    //main();
}

void os_tick(void) {
    MSP_SAVE(curr_task)
    save_partial_context(curr_task);
    restore_partial_context(curr_task);
    MSP_RESTORE(curr_task)
    asm("bx lr");
}

tcb* create_context(void(*func)(void)) {
    tcb* new_tcb = alloc(sizeof(tcb));
    uint32_t* new_stack = alloc(STACK_SIZE);
    new_tcb->stack_top = (uint32_t*)((uint32_t)new_stack + 100);
    new_tcb->stack_base = new_tcb->stack_top;
    new_tcb->stack_limit = new_stack;
    uint32_t reg_default = 0;
    uint32_t xpsr_default = 0x01000000;
    PUSH(new_tcb->stack_top, xpsr_default)
    PUSH(new_tcb->stack_top, func)
    PUSH(new_tcb->stack_top, func)
    PUSH(new_tcb->stack_top, new_tcb->stack_base)
    PUSH(new_tcb->stack_top, reg_default)
    PUSH(new_tcb->stack_top, reg_default)
    PUSH(new_tcb->stack_top, reg_default)
    PUSH(new_tcb->stack_top, reg_default)
    return new_tcb;
}
