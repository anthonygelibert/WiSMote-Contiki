/* NOT_YET_DOCUMENTED_PTV */

#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h> /* For printf() */

#define INTERVAL (CLOCK_CONF_SECOND*10)

static char remote[] = "192.168.1.1";
static uint16_t remote_port = 1234, local_port = 1235;

/*---------------------------------------------------------------------------*/
PROCESS(ping_process, "Ping process");
PROCESS(echo_process, "ECHO process");
AUTOSTART_PROCESSES(&ping_process, &echo_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ping_process, ev, data)
{
  static uip_ipaddr_t serveraddr;
  static struct uip_udp_conn *udpconn;
  static struct etimer et;

  PROCESS_BEGIN();

  uiplib_ipaddrconv(remote, &serveraddr);
  udpconn = udp_new(&serveraddr, uip_htons(remote_port), NULL);
  udp_bind(udpconn,uip_htons(local_port));
  while (1)
  {
    etimer_set(&et, INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    uip_udp_packet_send(udpconn, "Hello world !!!", 16);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(echo_process, ev, data)
{
  static uip_ipaddr_t localaddr;
  static struct uip_udp_conn *localconn;

  PROCESS_BEGIN();

  uiplib_ipaddrconv(remote, &localaddr);
  localconn = udp_new(&uip_all_zeroes_addr, 0, NULL);
  udp_bind(localconn,uip_htons(local_port));
  printf("IP: %u.%u.%u.%u\nLP: %u\nRP: %u\nTTL: %u\n",
      uip_ipaddr_to_quad(&(localconn->ripaddr)),
      uip_ntohs(localconn->lport),
      uip_ntohs(localconn->rport),
      localconn->ttl);
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    if(uip_newdata()) {
      {
        int i = 0;
        for (i = 0; i < uip_datalen(); i++)
        {
          printf("%c",((char *)uip_appdata)[i]);
        }
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
