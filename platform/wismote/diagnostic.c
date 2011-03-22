/* From MSP430-GCC */
#include <stdio.h>
/* From CONTIKI */
#include "contiki.h"

#define DEBUG 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static const uint16_t local_port = 7890;
static const uint16_t remote_port = 7891;
static struct uip_udp_conn *localconn;
static struct uip_udp_conn *remoteconn;

static char * getVersion(void)
{

}

static char * getProcess(void)
{

}

static char * getSensors(void)
{

}

static char * getError(void)
{
  return "Unknown command";
}

static const int HANDLERS_SIZE = 3;

static void * handlers[HANDLERS_SIZE][2] = {{"version", getVersion},
                                            {"process", getProcess},
                                            {"sensors", getSensors}};

/**
 * Send the answer.
 */
static void sendAnswer(const char const * answer)
{
  remoteconn = udp_new(localconn->ripaddr, uip_htons(remote_port), NULL);
  uip_udp_packet_send(udpconn, answer, sizeof(answer));
}

/**
 * Handler of diagnostic request.
 */
static void requestHandler(void)
{
  int i = 0;

  ((char *)uip_appdata)[uip_datalen()] = 0;
  PRINTF("New uIP data: '%s'\n", uip_appdata);
  while(i < HANDLERS_SIZE && strcmp(uip_appdata, handlers[i][0]));
  sendAnswer((i == HANDLERS_SIZE ? getError() : handlers[i][1]));
}

/*---------------------------------------------------------------------------*/
PROCESS(diagnostic_process, "Diagnostic");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(diagnostic_process, ev, data)
{
  /* Create the local listener */
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
}
