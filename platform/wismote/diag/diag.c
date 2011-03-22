/* From MSP430-GCC */
#include <stdio.h>
#include <string.h>

/* From CONTIKI */
#include "contiki.h"
#include "contiki-net.h"

/* From platform */
#include "diag.h"
#include "diag-version.h"
#include "diag-process.h"
#include "diag-sensor.h"

#define DEBUG 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define COMMANDS(...) static struct command *handlers[] = {__VA_ARGS__}

COMMANDS(&version_command, &sensor_command, &process_command);

static const int COMMAND_NUM = (sizeof(handlers) / sizeof(struct cmdHandler *));

static const uint16_t local_port = 7890;
static const uint16_t remote_port = 7891;
static struct uip_udp_conn *localconn;
static struct uip_udp_conn *remoteconn;

/**
 * Send the answer.
 */
static void sendAnswer(const char const * answer)
{
  PRINTF("I will answer '%s'\n", answer);
  uip_ipaddr_copy(&remoteconn->ripaddr, &UIP_IP_BUF->srcipaddr);
  uip_udp_packet_send(remoteconn, answer, strlen(answer));
}

/**
 * Handler of diagnostic request.
 */
static void requestHandler(void)
{
  int i = 0;

  ((char *)uip_appdata)[uip_datalen()] = 0;
  PRINTF("New uIP data: '%s'\n", (char *)uip_appdata);
  for (i = 0; i < COMMAND_NUM && strcmp(handlers[i]->name, uip_appdata); i++);
  if (i < COMMAND_NUM)
  {
    PRINTF("I chose handler '%s'\n", handlers[i]->name);
    sendAnswer(handlers[i]->handler());
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(diagnostic_process, "Diagnostic");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(diagnostic_process, ev, data)
{
  PROCESS_BEGIN();

  /* Create the local listener */
  remoteconn = udp_new(&uip_all_zeroes_addr, uip_htons(remote_port), NULL);
  localconn = udp_new(&uip_all_zeroes_addr, 0, NULL);
  udp_bind(localconn,uip_htons(local_port));
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    /* If there is new uIP data, display it */
    if(uip_newdata()) {
      requestHandler();
    }
  }

  PROCESS_END();
}
