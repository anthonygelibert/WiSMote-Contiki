/******************************************************************************
 * Copyright 2010, Plateforme Technologique de Valence                        *
 *                                                                            *
 *    Licensed under the Apache License, Version 2.0 (the "License");         *
 *    you may not use this file except in compliance with the License.        *
 *    You may obtain a copy of the License at                                 *
 *                                                                            *
 *        http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                            *
 *    Unless required by applicable law or agreed to in writing, software     *
 *    distributed under the License is distributed on an "AS IS" BASIS,       *
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *    See the License for the specific language governing permissions and     *
 *    limitations under the License.                                          *
 ******************************************************************************/
/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2010
 * \version 0.0.1
 */
#include <io.h>
#include <signal.h>

#define __C_RTIMER_ARCH_H__
#include "rtimer-arch.h"
#include "sys/energest.h"
#include "sys/rtimer.h"
#include "sys/process.h"
#include "dev/watchdog.h"

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

/** Initialize the Real-Timer. */
void rtimer_arch_init(void)
{
    dint();
    /* CCR0 interrupt enabled, interrupt occurs when timer equals CCR0. */
    TA1CCTL0 = CCIE;
    /* Enable interrupts. */
    eint();
}

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @return NOT_YET_DOCUMENTED_PTV
 */
rtimer_clock_t rtimer_arch_now(void)
{
    rtimer_clock_t t1, t2;
    do
    {
        /* TODO_PTV check TAR -> TA1R */
        t1 = TA1R;
        t2 = TA1R;
    }
    while (t1 != t2);
    return t1;
}

/** Set the delay before the next Real-Timer interruption.
 *
 * \param t Delay before next interruption.
 */
void rtimer_arch_schedule(rtimer_clock_t t)
{
    TA1CCR0 = t;
}
