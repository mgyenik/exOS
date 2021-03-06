#include <kernel.h>
#include <systick.h>
#include <main.h>

extern uint32_t _sheap;
extern uint32_t _eheap;

#define STACK_SIZE 128


struct task_list {
    tcb_node* head;
    tcb_node* tail;
} tlist = {
    .head = NULL,
    .tail = NULL
};

tcb_node* curr_task = NULL;

unsigned char switching = 0;

void schedule(void) {
    curr_task = curr_task->next;
}

void os_start(void) {
    /* Initialize heap so you can have dynamic memory allocation */
    init_region_as_dmem(&_sheap, &_eheap);

    /*Hardware/peripherals setup */
    init_leds();

    /* Startup task switching with the main user task, and make task list circular */
    tlist.head = create_task_node(&main, 0);
    tlist.head->next = tlist.head;

    /* Now we make curr_task valid, start the systick, and wait.... */
    curr_task = tlist.head;
    init_systick();
    while(1){};    
}

void os_tick(void) {
    /* Ghettohax since yield isn't implemented yet */
    if(!switching) {
        if(curr_task != NULL) {
            switching = 1;
            RESTORE_PARTIAL_CONTEXT(curr_task->task)
            MSP_RESTORE(curr_task)
            EXCEPT_RETURN()
        }
    }
    /* </ghettohax> */
    MSP_SAVE(curr_task)
    SAVE_PARTIAL_CONTEXT(curr_task->task)

    schedule();

    RESTORE_PARTIAL_CONTEXT(curr_task->task)
    MSP_RESTORE(curr_task)
    EXCEPT_RETURN()
}

tcb* create_task(void(*func)(void)) {
    tcb* new_tcb = alloc(sizeof(tcb));
    uint32_t* new_stack = alloc(STACK_SIZE);
    new_tcb->stack_top = (uint32_t*)((uint32_t)new_stack + 100);
    new_tcb->stack_base = new_tcb->stack_top;
    new_tcb->stack_limit = new_stack;
    uint32_t reg_default = 0;
    uint32_t xpsr_default = 0x21000000;
    *new_tcb->stack_top-- = xpsr_default;
    *new_tcb->stack_top-- = (uint32_t)func;
    *new_tcb->stack_top-- = (uint32_t)func;
    *new_tcb->stack_top-- = (uint32_t)new_tcb->stack_base;
    *new_tcb->stack_top-- = reg_default + 3;
    *new_tcb->stack_top-- = reg_default + 2;
    *new_tcb->stack_top-- = reg_default + 1;
    *new_tcb->stack_top = reg_default;
    return new_tcb;
}

void task_create(void(*func)(void), uint32_t jiffies) {
    task_insert(tlist.head, create_task_node(func, jiffies));
}

tcb_node* create_task_node(void(*func)(void), uint32_t jiffies) {
    tcb_node* tmp_task_node = (tcb_node*)alloc(sizeof(tcb_node));
    tmp_task_node->task = (tcb*)create_task(func);
    tmp_task_node->task->period = jiffies;
    return tmp_task_node;
}

/*
void restore_partial_context(tcb* task) {
    asm volatile("\
            add     %[task_addr], %[task_addr], %[context_offset]   \r\n\
            ldmia   %[task_addr], {r4-r11}                          \r\n\
        "
        :[task_addr] "+l" (task)
        :[context_offset] "I" (__builtin_offsetof(tcb, saved_partial_context))
        :"r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "memory"
    );
} 

void save_partial_context(tcb* task) {
    asm volatile("\
            add     %[task_addr], %[task_addr], %[context_offset]   \r\n\
            stmia   %[task_addr], {r4-r11}                          \r\n\
        "
        :[task_addr] "+l" (task)
        :[context_offset] "I" (__builtin_offsetof(tcb, saved_partial_context))
        :"memory"
    );
}
*/
void task_insert(tcb_node* head, tcb_node* new) {
    if(head != NULL) {
        new->next = head->next;
        head->next = new;
    }
}

void task_remove(tcb_node* head, tcb_node* del) {
    if(head == NULL){
        return;
    }
    tcb_node* walk = head;
    while(walk->next != NULL) {
        if(walk->next == del) {
            walk->next = walk->next->next;
        }
    }
    free(del);
}
