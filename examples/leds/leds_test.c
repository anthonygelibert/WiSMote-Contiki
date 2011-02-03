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
/*
void
clock_delay(unsigned int i)
{
  asm("add #-1, r15");
  asm("jnz $-2");
 
}
*/



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
/*
  P7SEL |= BIT0 + BIT1;						// Activate XT1
  
  UCSCTL6 &= ~XT1OFF;                       // Set XT1 On
  UCSCTL6 |= XT1DRIVE_2 + XTS + XT2OFF;     // Max drive strength, adjust
 */                                          // according to crystal frequency.
                                            // LFXT1 HF mode

  //leds_on(LEDS_YELLOW | LEDS_RED | LEDS_GREEN);
  //P2OUT &= ~BIT4;
  //P2OUT |= BIT4;

 while (1) {
  leds_off(LEDS_YELLOW | LEDS_RED | LEDS_GREEN);  
  delay();
  leds_on(LEDS_YELLOW | LEDS_RED | LEDS_GREEN);  
  delay();
/*
  leds_off(LEDS_YELLOW | LEDS_RED | LEDS_GREEN);  
  leds_on(LEDS_YELLOW);
  delay();
  leds_on(LEDS_RED);
  delay();
  leds_on(LEDS_GREEN);
  delay();
  leds_off(LEDS_YELLOW);
  delay();
  leds_off(LEDS_RED);
  delay();
  leds_off(LEDS_GREEN);*/
  }


  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
