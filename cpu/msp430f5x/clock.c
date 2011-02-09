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
 * \date Feb 3, 2011
 * \version 0.0.2
 */

#include "contiki-conf.h"
#include "sys/clock.h"
#include "signal.h"

/** NOT_YET_DOCUMENTED_PTV */
extern void
clock_platform_init(void);

/** NOT_YET_DOCUMENTED_PTV */
static void
initXT1(void);

/** NOT_YET_DOCUMENTED_PTV */
static void
initClockModule(void);

/** NOT_YET_DOCUMENTED_PTV */
void
clock_init(void)
{
  initXT1();
  initClockModule();
}

/** NOT_YET_DOCUMENTED_PTV */
void
clock_delay(unsigned int i)
{
  asm("add #-1, r15");
  asm("jnz $-2");
}

/** NOT_YET_DOCUMENTED_PTV */
static void
initXT1(void)
{
  dint();
  clock_platform_init();
  /* Set XT1 On */
  UCSCTL6 &= ~XT1OFF;
  /* Max drive strength, adjust according to crystal frequency. LFXT1 HF mode */
  UCSCTL6 |= XT1DRIVE_2 | XTS | XT2OFF;

  /* Loop while oscillator fault flag. */
  do {
    /* Clear XT2,XT1,DCO fault flags */
    UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);
    /* Clear fault flags */
    SFRIFG1 &= ~OFIFG;
  }
  while (SFRIFG1 & OFIFG);

  /* MCLCK = XT1 , SMCLK = XT1 , ACLK = XT1 */
  UCSCTL4 = SELA__XT1CLK | SELS__XT1CLK | SELM__XT1CLK;
  /* Set Clock divider 4 -> Clock = 4MHz */
  UCSCTL5 |= DIVA__4 | DIVS__4 | DIVM__4;
  eint();
}

/** NOT_YET_DOCUMENTED_PTV */
static void
initClockModule(void)
{
   // TODO_PTV
}
}
