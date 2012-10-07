#include "registers.h"
#include "main.h"
#include "mem.h"
#include "sched.h"

extern uint32_t _sheap;
extern uint32_t _eheap;

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
        delay = 2000000;
        while(delay--){
            asm volatile("nop");
        }
        LED_ODR ^= (1 << RED);
    }
}

void blink2(void) {
    long delay;
    while(1){
        delay = 1000000;
        while(delay--){
            asm volatile("nop");
        }
        LED_ODR ^= (1 << BLUE);
    }
}
