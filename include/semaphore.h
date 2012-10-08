

typedef volatile struct semaphore {
    uint8_t     lock;
    uint16_t    count;
    tcb*        owner;
} semaphore;

void release(semaphore* sem);
void acquire(semaphore* sem);
