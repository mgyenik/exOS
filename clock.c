#include <kernel.h>
#include <reg.h>

typedef struct _ClockConfiguration
{

    /** Processor clock frequency (in MHz). */
    uint16_t pck;
    /** Master clock frequency (in MHz). */
    uint16_t mck;
    /** CKGR_PLL reqister value. */
    uint32_t pllr;
    /** PMC_MCKR register value. */
    uint32_t mckr;
} ClockConfiguration ;

/* Clock configurations for the AT91SAM3S4-EK */
#define CKGR_MUL_SHIFT         16
#define CKGR_PLLCOUNT_SHIFT     8
#define CKGR_DIV_SHIFT          0

static const ClockConfiguration clockConfigurations[] = {

    /* PCK = 24 MHz, MCK = 24 MHz
     * PCK = 12000000 * (7+1) / 2 / 2 = 24 MHz
     */
    {24, 24, (CKGR_PLLAR_STUCKTO1 | (7 << CKGR_MUL_SHIFT) \
        | (0x3f << CKGR_PLLCOUNT_SHIFT) \
        | (2 << CKGR_DIV_SHIFT)),
        ( PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)},
    /* PCK = 48 MHz, MCK = 48 MHz
     * PCK = 12000000 * (7+1) / 1 / 2 = 48 MHz
     */
    {48, 48, (CKGR_PLLAR_STUCKTO1 | (7 << CKGR_MUL_SHIFT) \
        | (0x3f << CKGR_PLLCOUNT_SHIFT) \
        | (1 << CKGR_DIV_SHIFT)),
        ( PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)},
    /* PCK = 64 MHz, MCK = 64 MHz
     * PCK = 12000000 * (15+1) / 3 / 1 = 64 MHz
     */
    {64, 64, (CKGR_PLLAR_STUCKTO1 | (15 << CKGR_MUL_SHIFT) \
        | (0x3f << CKGR_PLLCOUNT_SHIFT) \
        | (3 << CKGR_DIV_SHIFT)),
        ( PMC_MCKR_PRES_CLK | PMC_MCKR_CSS_PLLA_CLK)}
};

void clocks(void) {
    int configuration = 2;

    /* Switch to main oscillator in two operations */
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (uint32_t)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0);

    /* Configure PLL */
    PMC->CKGR_PLLAR = clockConfigurations[configuration].pllr;
    while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0);

    /* Configure master clock in two operations */
    PMC->PMC_MCKR = (clockConfigurations[configuration].mckr & (uint32_t)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0);
    PMC->PMC_MCKR = clockConfigurations[configuration].mckr;
    while ((PMC->PMC_SR & PMC_SR_MCKRDY) == 0);

    /* DBGU reconfiguration */
}
