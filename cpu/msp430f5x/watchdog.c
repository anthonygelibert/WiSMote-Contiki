/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
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
 *
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: watchdog.c,v 1.12 2010/11/12 15:54:41 nifi Exp $
 */
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
