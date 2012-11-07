#include "registers.h"
#include "systick.h"

void init_systick(void) {
    /* 250us  at 168Mhz */
    SYSTICK_RELOAD = 16800;
    SYSTICK_VAL = 0;
    /* Use processor clock, enable interrupt, enable systick peripheral */
    SYSTICK_CTL = 0x00000007;
}
