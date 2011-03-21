/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \file
 *         Clock on UART0.
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

#include <stdio.h>
#include "contiki.h"

static int current_second = 0;
static int current_min = 0;
static int current_hour = 0;

/*---------------------------------------------------------------------------*/

static void
timeout_handler(void)
{
  if ((++current_second) >= 60) {
    current_second = 0;
    if ((++current_min) >= 60) {
      current_min = 0;
      current_hour++;
    }
  }
  printf("%02d:%02d:%02d\n", current_hour, current_min, current_second);
}

/*---------------------------------------------------------------------------*/
PROCESS(myTimer_process, "Timer Process");
AUTOSTART_PROCESSES(&myTimer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(myTimer_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  printf("I start the timer (1 s.)\n");
  while(1) {
    etimer_set(&et, CLOCK_CONF_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    timeout_handler();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

/** @} */
