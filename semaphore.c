#include <kernel.h>

static int atomic_get_lock(volatile uint8_t* lock) {
    /* Same code I wrote for F4OS */
    uint8_t taken = 1;
    uint8_t ret;
    uint8_t tmp = 0;
    asm volatile("\
        ldrexb      %[tmp], [%[addr]]             \r\n\
        cmp         %[tmp], #0                    \r\n\
        ITT         EQ                            \r\n\
        strexbeq    %[tmp], %[taken], [%[addr]]   \r\n\
        cmpeq       %[tmp], #0                    \r\n\
        ITE         EQ                            \r\n\
        moveq       %[ret], #1                    \r\n\
        movne       %[ret], #0\
        "
        :[ret] "=l" (ret)
        :[addr] "l" (lock), [tmp] "l" (tmp), [taken] "l" (taken)
        :"cc", "memory"
    );
    return ret;
}

void acquire(semaphore* sem) {
    while(!atomic_get_lock(&sem->lock)) {
        svc(SVC_YIELD);
    }
    return;
}

void release(semaphore* sem) {
    sem->lock = 0;
}
