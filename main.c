#include <kernel.h>
#include <main.h>
#include <semaphore.h>
#include <svc.h>

semaphore test_sem = {
    .lock = 0,
    .count = 0,
    .owner = NULL
};

void init_leds(void){
    RCC_AHB1ENR |= (1 << 3);
    GPIOD_MODER |= (1 << (RED*2)) | (1 << (BLUE*2));
}

void main(void){
    task_create(&blink1, 0);
    task_create(&blink2, 0);
    while(1){
        asm volatile("nop");
    }
}

void blink1(void) {
    long delay;
    while(1){
        acquire(&test_sem);
        for(int i = 10; i; i--) {
            delay = 2000000;
            while(delay--){
                asm volatile("nop");
            }
            LED_ODR ^= (1 << RED);
        }
        release(&test_sem);
        svc(SVC_YIELD);
    }
}

void blink2(void) {
    long delay;
    while(1){
        acquire(&test_sem);
        for(int i = 10; i; i--) {
            delay = 1000000;
            while(delay--){
                asm volatile("nop");
            }
            LED_ODR ^= (1 << BLUE);
        }
        release(&test_sem);
        svc(SVC_YIELD);
    }
}
