/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 */

#include "sys/process.h"
#include "sys/autostart.h"

#include <stdio.h> /* For printf() */

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Hello, world\n");
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
