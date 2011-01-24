#include <io.h>
#include <signal.h>

#include "sys/energest.h"
#include "sys/rtimer.h"
#include "sys/process.h"
#include "dev/watchdog.h"

#define DEBUG 0

#if DEBUG
    #include <stdio.h>
    #define PRINTF(...) printf(__VA_ARGS__)
#else
    #define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
interrupt(TIMER1_A0_VECTOR) timera1(void)
{
    ENERGEST_ON(ENERGEST_TYPE_IRQ);

    watchdog_start();

    rtimer_run_next();

    if (process_nevents() > 0)
    {
        LPM4_EXIT;
    }

    watchdog_stop();

    ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/
void rtimer_arch_init(void)
{
    dint();

    /* CCR0 interrupt enabled, interrupt occurs when timer equals CCR0. */
    TA1CCTL0 = CCIE;

    /* Enable interrupts. */
    eint();
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t rtimer_arch_now(void)
{
    rtimer_clock_t t1, t2;
    do
    {
        /* TODO: check TAR -> TA1R */
        t1 = TA1R;
        t2 = TA1R;
    }
    while (t1 != t2);
    return t1;
}
/*---------------------------------------------------------------------------*/
void rtimer_arch_schedule(rtimer_clock_t t)
{
    PRINTF("rtimer_arch_schedule time %u\n", t);

    TA1CCR0 = t;
}
/*---------------------------------------------------------------------------*/
