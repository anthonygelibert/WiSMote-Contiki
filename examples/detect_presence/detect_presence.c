/**
 * \file
 *         Button with event example.
 * \author
 *         Anthony Gelibert <anthony.gelibert@me.com>
 * \date
 *         Feb 18, 2011
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
#include "sys/process.h"
#include "sys/autostart.h"

#include "iohandlers.h"
#include "leds.h"
#include "clock.h"
#include <stdint.h>

/*---------------------------------------------------------------------------*/
/*
static void
delay(void)
{
  volatile uint16_t i;
  for (i = 0; i < 640; i++) {
    clock_delay(i);
  }
}*/

/*---------------------------------------------------------------------------*/

static void
myHandler(void)
{
 // leds_toggle(LEDS_GREEN);
  leds_toggle(LEDS_RED);
}

/*---------------------------------------------------------------------------*/
//
HWCONF_PIN(BUTTON, 1, 2)
HWCONF_IRQ(BUTTON, 1, 2, myHandler)

/*---------------------------------------------------------------------------*/
PROCESS(detect_presence_process, "detect_presence process");
AUTOSTART_PROCESSES(&detect_presence_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(detect_presence_process, ev, data)
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


// TODO: A fusionner avec les init_button_*
        // INIT PORT
//    P1REN |= BIT2;  // Enable P1.2 internal resistance
//    P1OUT &= ~BIT2; // Set P1.2 as pull-Down resistance //mettre le pulldown (forcer état à 0)
//    P1IES &= ~BIT2; // P1.2 Lo/Hi edge   //configurer interruption sur 0->1
//    P1IE |= BIT2;   // P1.2 interrupt enabled
//    P1IFG &= ~BIT2; // P1.2 IFG cleared

        // INIT LED
        //P8DIR |= BIT6; // P8.6 output VERTE
        //P5DIR |= BIT2; // P5.2 output ROUGE
        //P2DIR |= BIT4; // P2.4 output BLUE

        // INIT CAPT
        P1DIR &= ~BIT2; // Set P1.2 to input direction   ENTREE SIGNAL CAPT
        P6DIR |= BIT0; // P6.0 ADC0 output      GND
        P6OUT &= ~BIT0; // P6.0 ADC0 output     GND

     //   __bis_SR_register(LPM0_bits | GIE);

    while (1)
    {
      PROCESS_PAUSE();
      //leds_on(LEDS_BLUE);
      //delay();
      //leds_off(LEDS_BLUE);
      //delay();
    }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
