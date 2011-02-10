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
#include "dev/watchdog.h"
#include "sys/clock.h"
#include "sys/energest.h"
#include "sys/etimer.h"
#include "signal.h"
#include "rtimer-arch.h"

/** NOT_YET_DOCUMENTED_PTV */
#define INTERVAL (RTIMER_ARCH_SECOND / CLOCK_SECOND)
/** NOT_YET_DOCUMENTED_PTV */
#define MAX_TICKS (~((clock_time_t)0) / 2)
/** NOT_YET_DOCUMENTED_PTV */
static volatile unsigned long seconds;
/** NOT_YET_DOCUMENTED_PTV */
static volatile clock_time_t count = 0;
/** Used for calculating clock_fine. */
static volatile uint16_t last_tar = 0;

/** NOT_YET_DOCUMENTED_PTV */
extern void
clock_platform_init(void);

/** NOT_YET_DOCUMENTED_PTV */
static void
initXT1(void);

/** NOT_YET_DOCUMENTED_PTV */
static void
initClockModule(void);

/**
 * Wait for a multiple of 10 ms.
 *
 */
void
clock_wait(int i)
{
  clock_time_t start;

  start = clock_time();
  while (clock_time() - start < (clock_time_t) i) {
  }
}

/** NOT_YET_DOCUMENTED_PTV */
interrupt(TIMER1_A1_VECTOR)
timera1(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

  if (TA1IV == 2) {

    /* Make sure interrupt time is future */
    do {
      TA1CCR1 += INTERVAL;
      ++count;

      /* Make sure the CLOCK_CONF_SECOND is a power of two, to ensure
       that the modulo operation below becomes a logical and and not
       an expensive divide. Algorithm from Wikipedia:
       http://en.wikipedia.org/wiki/Power_of_two */
#if (CLOCK_CONF_SECOND & (CLOCK_CONF_SECOND - 1)) != 0
#error CLOCK_CONF_SECOND must be a power of two (i.e., 1, 2, 4, 8, 16, 32, 64, ...).
#error Change CLOCK_CONF_SECOND in contiki-conf.h.
#endif
      if (count % CLOCK_CONF_SECOND == 0) {
        ++seconds;
        energest_flush();
      }
    }
    while ((TA1CCR1 - TA1R) > INTERVAL);

    last_tar = TA1R;

    if (etimer_pending() && (etimer_next_expiration_time() - count - 1)
        > MAX_TICKS) {
      etimer_request_poll();
      LPM4_EXIT;
    }
  }
  watchdog_stop();

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/** NOT_YET_DOCUMENTED_PTV */
clock_time_t
clock_time(void)
{
  clock_time_t t1, t2;
  do {
    t1 = count;
    t2 = count;
  }
  while (t1 != t2);
  return t1;
}

/** NOT_YET_DOCUMENTED_PTV */
void
clock_set(clock_time_t clock, clock_time_t fclock)
{
  TA1R = fclock;
  TA1CCR1 = fclock + INTERVAL;
  count = clock;
}

/** NOT_YET_DOCUMENTED_PTV */
int
clock_fine_max(void)
{
  return INTERVAL;
}

/** NOT_YET_DOCUMENTED_PTV */
unsigned short
clock_fine(void)
{
  unsigned short t;
  /* Assign last_tar to local variable that can not be changed by interrupt */
  t = last_tar;
  /* perform calculation based on t, TAR will not be changed during interrupt */
  return (unsigned short) (TA1R - t);
}

/** NOT_YET_DOCUMENTED_PTV */
void
clock_set_seconds(unsigned long sec)
{
}

/** NOT_YET_DOCUMENTED_PTV */
unsigned long
clock_seconds(void)
{
  unsigned long t1, t2;
  do {
    t1 = seconds;
    t2 = seconds;
  }
  while (t1 != t2);
  return t1;
}

/** NOT_YET_DOCUMENTED_PTV */
rtimer_clock_t
clock_counter(void)
{
  return TA1R;
}

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
  /* Set Clock divider 2 -> Clock = 2 MHz (ACLK = 2 MHz) */
  UCSCTL5 |= DIVA__8 | DIVS__4 | DIVM__4;
  eint();
}

/** NOT_YET_DOCUMENTED_PTV */
static void
initClockModule(void)
{
  dint();

  //TA1CTL = TASSEL__SMCLK | TACLR | ID__2;
  TA1EX0 |= TAIDEX_7;
  TA1CTL = TASSEL__ACLK | TACLR | ID__8;

  /* Initialize ccr1 to create the X ms interval. */
  /* CCR1 interrupt enabled, interrupt occurs when timer equals CCR1. */
  TA1CCTL1 |= CCIE;

  /* Interrupt after X ms. */
  TA1CCR1 = INTERVAL;

  /* Start Timer_A in continuous mode. */
  TA1CTL |= MC__CONTINOUS;
  count = 0;
  /* Enable interrupts. */
  eint();
}
