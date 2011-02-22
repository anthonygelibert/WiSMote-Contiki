/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Contiki main.
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

#include <stdio.h>
#include <signal.h>

#include "contiki.h"
#include "dev/leds.h"
#include "dev/serial-line.h"
#include "dev/button-sensor.h"
#include "dev/watchdog.h"
#include "dev/slip.h"
#include "lib/sensors.h"
#include "sys/energest.h"

#include "uart0.h"
#include "spl.h"
#include "msp430.h"

SENSORS(&button_sensor);

/** Display the list of auto-processes before executing them. */
#define DEBUG_PROCESS 0
/** Doesn't display the list of sensors before starting them. */
#define DEBUG_SENSORS 0

#if DEBUG_PROCESS
/*---------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------*/
#endif /* DEBUG_PROCESS */

#if DEBUG_SENSORS
/*---------------------------------------------------------------------------*/
static void
print_sensors(void)
{
  struct sensors_sensor * sensor = sensors_first();

  printf("Sensors");
  while (sensor) {
    printf(" '%s' ", sensor->type);
    sensor = sensors_next(sensor);
  }
  putchar('\n');
}
/*---------------------------------------------------------------------------*/
#endif /* DEBUG_SENSORS */

/*---------------------------------------------------------------------------*/

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

#if !CONTIKI_NO_NET && (WITH_UIP || WITH_UIP6) && SLIP_ENABLED
  slip_arch_init(0);
#endif
  leds_on(LEDS_GREEN);
  leds_off(LEDS_RED);

  /* Initialize the RTimer */
  rtimer_init();
  /* Initialize the "process system" (core/sys/process.h)     */
  process_init();
  /* Initialize the ETimer module */
  process_start(&etimer_process, NULL);
  /* Initialize the CTimer module */
  ctimer_init();
  /* Initialize the Serial Line module */
#if CONTIKI_NO_NET || (!WITH_UIP && !WITH_UIP6)
  uart0_set_input(serial_line_input_byte);
  serial_line_init();
#endif

#if !CONTIKI_NO_NET && WITH_UIP

#endif

  /* Initialize the EnerGest module */
  energest_init();
  /* SETUP : END */

  ENERGEST_ON(ENERGEST_TYPE_CPU);
  watchdog_start();

  printf(CONTIKI_VERSION_STRING " started.\n");

  /* Initialize the sensors */
#if DEBUG_SENSORS
  print_sensors();
#endif
  process_start(&sensors_process, NULL);

  /* Start the processes */
#if DEBUG_PROCESS
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

    /* Idle processing */
    int s = splhigh(); /* Disable interrupts. */
    /* uart1_active is for avoiding LPM3 when still sending or receiving */
    if (process_nevents() != 0 || uart0_active()) {
      /* Re-enable interrupts. */
      splx(s);
    } else {
      static unsigned long irq_energest = 0;
      /* Re-enable interrupts and go to sleep atomically. */
      ENERGEST_OFF(ENERGEST_TYPE_CPU);
      ENERGEST_ON(ENERGEST_TYPE_LPM);
      /* We only want to measure the processing done in IRQs when we
       are asleep, so we discard the processing time done when we
       were awake. */
      energest_type_set(ENERGEST_TYPE_IRQ, irq_energest);
      watchdog_stop();
      /* XXX_PTV Ici, il faut ajouter une mise en veille dans certain cas.
       * Voir la plateforme Sky.
       * _BIS_SR(LPM3_bits);
       */
      dint();
      irq_energest = energest_type_time(ENERGEST_TYPE_IRQ);
      eint();
      watchdog_start();
      ENERGEST_OFF(ENERGEST_TYPE_LPM);
      ENERGEST_ON(ENERGEST_TYPE_CPU);
    }
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

/** @} */
