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
 */
#include <io.h>
#include <signal.h>
#include "dev/watchdog.h"

/** NOT_YET_DOCUMENTED_PTV */
static int counter = 0;

/** NOT_YET_DOCUMENTED_PTV */
interrupt(WDT_VECTOR) watchdog_interrupt(void)
{
    watchdog_reboot();
}

/** NOT_YET_DOCUMENTED_PTV
 *  FIXME_PTV
 */
void watchdog_init(void)
{
    /* The MSP430 watchdog is enabled at boot-up, so we stop it during initialization. */
    counter = 0;
    watchdog_stop();

    IFG1 &= ~WDTIFG;
    IE1 |= WDTIE;
}

/** NOT_YET_DOCUMENTED_PTV */
void watchdog_start(void)
{
    /* We setup the watchdog to reset the device after one second,
     unless watchdog_periodic() is called. */
    counter--;
    if (counter == 0)
    {
        WDTCTL = WDTPW | WDTCNTCL | WDT_ARST_1000 | WDTTMSEL;
    }
}

/** NOT_YET_DOCUMENTED_PTV */
void watchdog_periodic(void)
{
    /* This function is called periodically to restart the watchdog timer. */
    /*  if(counter < 0) {*/
    WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL | WDTTMSEL;
    /*  }*/
}

/** NOT_YET_DOCUMENTED_PTV */
void watchdog_stop(void)
{
    counter++;
    if (counter == 1)
    {
        WDTCTL = WDTPW | WDTHOLD;
    }
}

/** NOT_YET_DOCUMENTED_PTV */
void watchdog_reboot(void)
{
    WDTCTL = 0;
}
