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
 * \author Anthony Gelibert
 * \date Feb 10, 2011
 * \version 0.0.2
 */

#include "contiki.h"
#include "sys/process.h"
#include "sys/autostart.h"
#include "watchdog.h"
#include "leds.h"
#include "signal.h"

#include <stdint.h>

void
delay(void)
{
  volatile uint16_t i;
  for (i = 0; i < 64000; i++) {
    asm volatile("nop");
  }
}

/** NOT_YET_DOCUMENTED_PTV */
interrupt(PORT1_VECTOR)
port1ITHandler(void)
{
  // User Int is P1.4
  if (P1IFG & BIT4)
  {
      // Toogle Green led
      leds_toggle(LEDS_GREEN);

      // Clear IFG
    //P1IFG &= ~BIT4;
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(leds_process, "LEDs process");
AUTOSTART_PROCESSES(&leds_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(leds_process, ev, data)
{
  PROCESS_BEGIN();

  //watchdog_stop();
  //watchdog_start();
  //leds_off(LEDS_BLUE | LEDS_RED | LEDS_GREEN);
  while (1) {
    leds_on(LEDS_BLUE);
    delay();
    leds_off(LEDS_BLUE);
    delay();

    PROCESS_YIELD();
    leds_on(LEDS_BLUE);
    delay();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
