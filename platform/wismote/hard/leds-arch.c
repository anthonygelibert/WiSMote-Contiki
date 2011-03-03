/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         LEDs platform-dependent code.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 03, 2011
 */

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

#include "contiki-conf.h"
#include "dev/leds.h"

#define PORTDIR_LED_BLUE  P2DIR
#define PORTDS_LED_BLUE   P2DS
#define PORTOUT_LED_BLUE  P2OUT
#define BIT_LED_BLUE      BIT4

#define PORTDIR_LED_RED   P5DIR
#define PORTDS_LED_RED    P5DS
#define PORTOUT_LED_RED   P5OUT
#define BIT_LED_RED       BIT2

#define PORTDIR_LED_GREEN P8DIR
#define PORTDS_LED_GREEN  P8DS
#define PORTOUT_LED_GREEN P8OUT
#define BIT_LED_GREEN     BIT6

/*---------------------------------------------------------------------------*/

/**
 * \brief Initialize the ports used by the LEDs.
 */
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

/*---------------------------------------------------------------------------*/

/**
 * \brief Get the state of the leds.
 *
 * \return State of the leds (in one byte).
 */
unsigned char leds_arch_get(void)
{
    return ((PORTOUT_LED_RED & BIT_LED_RED) ? 0 : LEDS_RED)
      | ((PORTOUT_LED_GREEN & BIT_LED_GREEN) ? 0 : LEDS_GREEN)
      | ((PORTOUT_LED_BLUE & BIT_LED_BLUE) ? 0 : LEDS_BLUE);
}

/*---------------------------------------------------------------------------*/

/**
 * \brief Set the state of the leds.
 *
 * \param leds New state of the leds (in one byte).
 */
void leds_arch_set(unsigned char leds)
{
    PORTOUT_LED_RED = (PORTOUT_LED_RED & ~BIT_LED_RED) | ((leds & LEDS_RED) ? 0 : BIT_LED_RED);
    PORTOUT_LED_GREEN = (PORTOUT_LED_GREEN & ~BIT_LED_GREEN) | ((leds & LEDS_GREEN) ? 0 : BIT_LED_GREEN);
    PORTOUT_LED_BLUE = (PORTOUT_LED_BLUE & ~BIT_LED_BLUE) | ((leds & LEDS_BLUE) ? 0 : BIT_LED_BLUE);
}

/*---------------------------------------------------------------------------*/

/** @} */
