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

#include "msp430.h"
#include "dev/watchdog.h"

#include <signal.h>

/** Ports initialization.
 *
 * Initialize all the ports of the platform:
 * <ul>
 *    <li>PxSEL = 0</li>
 *    <li>PxDIR = 0</li>
 *    <li>PxOUT = 0</li>
 *    <li>PxIE  = 0</li>
 * </ul>
 */
static void
init_ports(void)
{
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

  /* All available inputs */
#ifdef P1DIR
  P1DIR = 0;
  P1OUT = 0;
#endif
#ifdef P2DIR
  P2DIR = 0;
  P2OUT = 0;
#endif
#ifdef P3DIR
  P3DIR = 0;
  P3OUT = 0;
#endif
#ifdef P4DIR
  P4DIR = 0;
  P4OUT = 0;
#endif
#ifdef P5DIR
  P5DIR = 0;
  P5OUT = 0;
#endif
#ifdef P6DIR
  P6DIR = 0;
  P6OUT = 0;
#endif
#ifdef P7DIR
  P7DIR = 0;
  P7OUT = 0;
#endif
#ifdef P8DIR
  P8DIR = 0;
  P8OUT = 0;
#endif

  /* All available IE */
#ifdef P1IE
  P1IE = 0;
#endif
#ifdef P2IE
  P2IE = 0;
#endif
}

/**
 * NOT_YET_DOCUMENTED_PTV
 */
void
msp430_cpu_init(void)
{
  dint();
  watchdog_init();
  init_ports();
  msp430_init_clock();
  eint();
}
