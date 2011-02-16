/*
 * Copyright (c) 2011, Plateforme Technologique de Valence.
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
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
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
/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2011
 * \version 0.0.4
 */

#include <io.h>
#include <signal.h>
#include "dev/watchdog.h"

/** Counter for watchdog's cycle. */
static int s_counter = 0;

/** Reboot the watchdog timer.
 *
 *  This interrupt vector restart the watchdog timer.
 */
interrupt(WDT_VECTOR)
watchdog_interrupt(void)
{
  watchdog_reboot();
}

/**
 * Initialize the watchdog timer.
 *
 * \note The MSP430 watchdog is enabled at boot-up, so we stop it during initialization.
 */
void
watchdog_init(void)
{
  s_counter = 0;
  watchdog_stop();

  SFRIFG1 &= ~WDTIFG;
  SFRIE1 |= WDTIE;
}

/**
 * Start the watchdog timer.
 *
 * \note We setup the watchdog to reset the device after one second,
 *    unless watchdog_periodic() is called.
 */
void
watchdog_start(void)
{
  if ((--s_counter) == 0) {
    WDTCTL = WDTPW | WDTCNTCL | WDTIS1 | WDTTMSEL;
  }
}

/** Restart periodically the watchdog timer.
 *
 * This function is called periodically to restart the watchdog timer.
 */
void
watchdog_periodic(void)
{
  WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL | WDTTMSEL;
}

/** Stop the counter */
void
watchdog_stop(void)
{
  if ((++s_counter) == 1) {
    WDTCTL = WDTPW | WDTHOLD;
  }
}

/** Reboot the watchdog timer. */
void
watchdog_reboot(void)
{
  WDTCTL = 0;
}
