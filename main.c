#include "registers.h"
#include "main.h"
#include "mem.h"

extern uint32_t _sheap;
extern uint32_t _eheap;

void init_led(void){
    RCC_AHB1ENR |= (1 << 3);
    GPIOD_MODER |= (1 << (RED*2));
}

int main(void){
    init_led();
    char* data = alloc(4);
    data[0] = 1;
    data[1] = 0x11;
    data[3] = 0x44;
    while(1){
        LED_ODR |= (data[0] << RED);
        LED_ODR &= ~(1 << RED);
    }
}
