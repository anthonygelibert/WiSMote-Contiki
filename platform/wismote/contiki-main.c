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
 * \date Feb 09, 2011
 * \version 0.0.2
 */

#include <stdio.h>

#include "contiki.h"
#include "dev/leds.h"
#include "dev/watchdog.h"
#include "dev/button-sensor.h"
#include "uart0.h"
#include "msp430.h"

/** Display the list of auto-processes before executing them */
#define DEBUG_PROCESS

#ifdef DEBUG_PROCESS
/**
 * \brief Display a list of processes.
 *
 * \param processes An array of processes
 */
static void
print_processes(struct process * const processes[])
{
  printf("Starting");
  while(*processes != NULL) {
    printf(" '%s'", PROCESS_NAME_STRING(*processes));
    processes++;
  }
  putchar('\n');
}
#endif /* DEBUG_PROCESS */

SENSORS(&button_sensor);

/**
 * Make all the initializations and start the auto-processes.
 *
 * @return Always 0
 */
int
main(void)
{
  /* Initialize the msp430 */
  cpu_init();
  /* Initialize the clock of the card and the clock module */
  clock_init();
  /* Initialize the leds */
  leds_init();
  leds_on(LEDS_RED);

  /* Initialize the uart */
  /* See MSP430x5xx/6xx Family User's Guide p. 588 */
  uart0_init(34,UCBRS_3,UCBRF_0);
  leds_on(LEDS_GREEN);
  leds_off(LEDS_RED);

  /* Initialize the "process system" (core/sys/process.h)     */
  process_init();
  process_start(&sensors_process, NULL);

  /* SETUP : END */

  printf(CONTIKI_VERSION_STRING " started. ");
#ifdef DEBUG_PROCESS
  print_processes(autostart_processes);
#endif /* DEBUG_PROCESS */
  autostart_start(autostart_processes);

  while (1) {
    int r;

    do {
      watchdog_periodic();
      r = process_run();
    }
    while (r > 0);

    /* TODO_PTV: implement idle processing. */
  }
  return 0;
}

