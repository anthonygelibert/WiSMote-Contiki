/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Parallax PIR sensor with IT example.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 *         Fabien Rey <fabien-rey@wanadoo.fr>
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

#include "contiki.h"
#include "iohandlers.h"
#include "leds.h"

/*---------------------------------------------------------------------------*/

static void
myHandler(void)
{
  leds_toggle(LEDS_RED);
}

/*---------------------------------------------------------------------------*/

HWCONF_PIN(BUTTON, 1, 2)
HWCONF_IRQ(BUTTON, 1, 2, myHandler)

/*---------------------------------------------------------------------------*/
PROCESS(parallaxPIRIT, "Parallax PIR IT process");
AUTOSTART_PROCESSES(&parallaxPIRIT);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(parallaxPIRIT, ev, data)
{
  PROCESS_BEGIN();

  leds_off(LEDS_RED);
  leds_off(LEDS_GREEN);

  BUTTON_RESISTOR_ENABLE();
  BUTTON_CLEAR();
  BUTTON_IRQ_EDGE_SELECTU();
  BUTTON_SELECT();
  BUTTON_MAKE_INPUT();
  BUTTON_SET_HANDLER();
  BUTTON_ENABLE_IRQ();

  while (1)
  {
    PROCESS_PAUSE();
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

/** @} */
