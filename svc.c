#include <kernel.h>

void svc_handler(void) {
    uint32_t* saved_regs = NULL;
    MSP_SAVE(curr_task->task)
    save_partial_context(curr_task->task);
    asm volatile("          \
            mov %[regs], sp \
            "
            :[regs] "+l" (saved_regs)
            ::"memory");
    uint8_t svc_number = ((char*)saved_regs[6])[-2];
    switch(svc_number) {
        case SVC_YIELD:
            /* Do normal os_tick stuff */
            break;
        default:
            break;
    }
    restore_partial_context(curr_task->task);
    MSP_RESTORE(curr_task->task);
}
