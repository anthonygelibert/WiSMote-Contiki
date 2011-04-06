/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \addtogroup rtimer
 * @{
 */

/**
 * \file
 *         Low-level RTC implementation.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 21, 2011
 *
 * \todo   XXX_PTV Timer -> RTC
 */

/*
 * Copyright (c) 2011, LCIS/CTSYS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* From MSP430-GCC */
#include <io.h>
#include <signal.h>

/* From CONTIKI */
#include "contiki.h"

/* From MSP430x5xx */
#include "rtimer-arch.h"
#include "dev/watchdog.h"

/*---------------------------------------------------------------------------*/

interrupt(RTC_VECTOR)
rtcIT(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

  if (RTCIV & RTC_RTCTEVIFG) {
    rtimer_run_next();
    if (process_nevents() > 0) {
      LPM4_EXIT;
    }
  }

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/*---------------------------------------------------------------------------*/

/** Initialize the Real-Timer. */
void
rtimer_arch_init(void)
{
  dint();

  /* Enable:
   *  - Real-time clock time event interrupt
   *  - RT1PS clock
   *  - 16-bit overflow
   *  - Counter (by default)
   */
  RTCCTL01 = RTCTEVIE | RTCSSEL__RT1PS | RTCTEV_1;

  /* Set the PS0 clock:
   *  - ACLK -- [/8] --> RT0PS
   */
  RTCPS0CTL = RT0PSDIV_2;

  /* Set the PS1 clock:
   *  - RT0PS -- [/16] --> RT1PS
   */
  RTCPS1CTL = RT1SSEL_2 | RT1PSDIV_3;

  /* Enable interrupts. */
  eint();
}

/*---------------------------------------------------------------------------*/

rtimer_clock_t
rtimer_arch_now(void)
{
  rtimer_clock_t t1, t2;
  do {
    t1 = RTCNT12;
    t2 = RTCNT12;
  }
  while (t1 != t2);
  return t1;
}

/*---------------------------------------------------------------------------*/

/** Set the delay before the next Real-Timer interruption.
 *
 * \param t Delay before next interruption.
 */
void
rtimer_arch_schedule(rtimer_clock_t t)
{
  /* The events occur on a 16-bits counter overflow. */
  RTCNT12 = (0xFFFF - t);
}

/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
