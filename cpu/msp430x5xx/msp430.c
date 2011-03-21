/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \file
 *         MSP430 initialization implementation.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 21, 2011
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
#include <signal.h>

/* From CONTIKI*/
#include "dev/watchdog.h"

/* From MSP430x5xx */
#include "contiki-conf.h"
#include "msp430.h"

/*---------------------------------------------------------------------------*/


static void
init_ports(void)
{
  /* See MSP430x5xx/MSP430x6xx Family p.320 */
  /* All configured for digital I/O */
#ifdef P1SEL
  P1SEL = 0;
#endif
#ifdef P2SEL
  P2SEL = 0;
#endif
#ifdef P3SEL
  P3SEL = 0;
#endif
#ifdef P4SEL
  P4SEL = 0;
#endif
#ifdef P5SEL
  P5SEL = 0;
#endif
#ifdef P6SEL
  P6SEL = 0;
#endif
#ifdef P7SEL
  P7SEL = 0;
#endif
#ifdef P8SEL
  P8SEL = 0;
#endif

  /* All available outputs */
#ifdef P1DIR
  P1DIR = 0;
#endif
#ifdef P2DIR
  P2DIR = 0;
#endif
#ifdef P3DIR
  P3DIR = 0;
#endif
#ifdef P4DIR
  P4DIR = 0;
#endif
#ifdef P5DIR
  P5DIR = 0;
#endif
#ifdef P6DIR
  P6DIR = 0;
#endif
#ifdef P7DIR
  P7DIR = 0;
#endif
#ifdef P8DIR
  P8DIR = 0;
#endif

  /* All available IE */
#ifdef P1IE
  P1IE = 0;
#endif
#ifdef P2IE
  P2IE = 0;
#endif
}

/*---------------------------------------------------------------------------*/

/** \brief      Initialize the MSP430 CPU.
 *
 *  This function initializes :
 *  <ol>
 *      <li>WatchDog</li>
 *      <li>Ports</li>
 *  </ol>
 */
void
msp430_cpu_init(void)
{
  dint();
  watchdog_init();
  init_ports();
  eint();
}

/*---------------------------------------------------------------------------*/

/** @} */
