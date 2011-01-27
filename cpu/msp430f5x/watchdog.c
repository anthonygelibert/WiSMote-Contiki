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
 * \version 0.0.3
 *
 * XXX_PTV: Check the usage of the counter...
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
    WDTCTL = WDTPW | WDTCNTCL | WDT_ARST_1000 | WDTTMSEL;
  }
}

/** Restart periodically the watchdog timer.
 *
 * This function is called periodically to restart the watchdog timer.
 */
void
watchdog_periodic(void)
{
  /*  if(counter < 0) {*/
  WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL | WDTTMSEL;
  /*  }*/
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
