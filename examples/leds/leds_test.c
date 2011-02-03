/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 */
#include "contiki.h"
#include "sys/process.h"
#include "sys/autostart.h"
#include "watchdog.h"
#include "leds.h"

#include <stdint.h>


void delay(void) {
    volatile uint16_t i;
    for (i = 0; i < 64000; i++) {
            asm volatile("nop");
    }
}

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  watchdog_stop();

 while (1) {
  leds_off(LEDS_BLUE | LEDS_RED | LEDS_GREEN);
  delay();
  leds_on(LEDS_BLUE | LEDS_RED | LEDS_GREEN);
  delay();
  }


  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
