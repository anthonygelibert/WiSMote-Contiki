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
 * \date Jan 24, 2010
 * \version 0.0.2
 */

#include "contiki-conf.h"
#include "dev/leds.h"

/** PxDIR used for the blue LED. */
#define PORTDIR_LED_BLUE  P2DIR
/** PxDS used for the blue LED. */
#define PORTDS_LED_BLUE   P2DS
/** PxOUT used for the blue LED. */
#define PORTOUT_LED_BLUE  P2OUT
/** BIT used for the blue LED. */
#define BIT_LED_BLUE      BIT4

/** PxDIR used for the red LED. */
#define PORTDIR_LED_RED   P5DIR
/** PxDS used for the red LED. */
#define PORTDS_LED_RED    P5DS
/** PxOUT used for the red LED. */
#define PORTOUT_LED_RED   P5OUT
/** BIT used for the red LED. */
#define BIT_LED_RED       BIT2

/** PxDIR used for the green LED. */
#define PORTDIR_LED_GREEN P8DIR
/** PxDS used for the green LED. */
#define PORTDS_LED_GREEN  P8DS
/** PxOUT used for the green LED. */
#define PORTOUT_LED_GREEN P8OUT
/** BIT used for the green LED. */
#define BIT_LED_GREEN     BIT6

/** Initialize the ports used by the LEDs. */
void leds_arch_init(void)
{
    PORTDIR_LED_GREEN |= BIT_LED_GREEN;
    PORTDIR_LED_RED   |= BIT_LED_RED;
    PORTDIR_LED_BLUE  |= BIT_LED_BLUE;

    PORTDS_LED_GREEN  |= BIT_LED_GREEN;
    PORTDS_LED_RED    |= BIT_LED_RED;
    PORTDS_LED_BLUE   |= BIT_LED_BLUE;

    PORTOUT_LED_BLUE |= BIT_LED_BLUE;
    PORTOUT_LED_GREEN |= BIT_LED_GREEN;
    PORTOUT_LED_RED |= BIT_LED_RED;
}

/**
 * Get the state of the leds.
 *
 * \return State of the leds (in one byte).
 */
unsigned char leds_arch_get(void)
{
    return ((PORTOUT_LED_RED & BIT_LED_RED) ? 0 : LEDS_RED)
      | ((PORTOUT_LED_GREEN & BIT_LED_GREEN) ? 0 : LEDS_GREEN)
      | ((PORTOUT_LED_BLUE & BIT_LED_BLUE) ? 0 : LEDS_BLUE);
}

/**
 * Set the state of the leds.
 *
 * \param leds New state of the leds (in one byte).
 */
void leds_arch_set(unsigned char leds)
{
    PORTOUT_LED_RED = (PORTOUT_LED_RED & ~BIT_LED_RED) | ((leds & LEDS_RED) ? 0 : BIT_LED_RED);
    PORTOUT_LED_GREEN = (PORTOUT_LED_GREEN & ~BIT_LED_GREEN) | ((leds & LEDS_GREEN) ? 0 : BIT_LED_GREEN);
    PORTOUT_LED_BLUE = (PORTOUT_LED_BLUE & ~BIT_LED_BLUE) | ((leds & LEDS_BLUE) ? 0 : BIT_LED_BLUE);
}
