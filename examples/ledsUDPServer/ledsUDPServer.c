/* NOT_YET_DOCUMENTED_PTV */

#include "contiki.h"
#include "contiki-net.h"
#include "dev/leds.h"
#include <string.h>
#include <stdio.h> /* For printf() */

#define DEBUG 1

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static const uint16_t local_port = 1235;
static const char preamble[] = "LED-";
static const uint8_t preambleSize = 4;

/*---------------------------------------------------------------------------*/
PROCESS(echo_process, "LEDS UDP Server Process");
AUTOSTART_PROCESSES(&echo_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(echo_process, ev, data)
{
  static struct uip_udp_conn *localconn;

  PROCESS_BEGIN();

  localconn = udp_new(&uip_all_zeroes_addr, 0, NULL);
  udp_bind(localconn,uip_htons(local_port));
  PRINTF("I listen the UDP port %u\n",uip_ntohs(localconn->lport));
  while (1)
  {

    PRINTF("I wait a new request\n");
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_newdata()) {
      if (!strncmp(uip_appdata,preamble,preambleSize))
      {
        PRINTF("I received a LED toggling request.\n");
        {
          char *data = (char *)uip_appdata;
          int led = 0;
          switch (data[4]) {
            case '1': led = LEDS_BLUE; break;
            case '2': led = LEDS_GREEN; break;
            case '3': led = LEDS_RED; break;
            default : PRINTF("I can't find the good LED...\n"); continue;
          }
          leds_toggle(led);
        }
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
