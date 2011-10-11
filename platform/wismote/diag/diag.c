/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Button sensor on Wismote.
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
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS "AS IS" AND
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

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define COMMANDS(...) static struct command *handlers[] = {__VA_ARGS__}

COMMANDS(&version_command, &sensor_command, &process_command);

/** Number of enabled commands. */
static const int COMMAND_NUM = (sizeof(handlers) / sizeof(struct cmdHandler *));

/** The uIP buffer. */
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
/** The local port. */
static const uint16_t local_port = 7890;
/** The remote port. */
static const uint16_t remote_port = 7891;
/** The local connection. */
static struct uip_udp_conn *localconn;
/** The remote connection. */
static struct uip_udp_conn *remoteconn;

#if UIP_CONF_IPV6
const uip_ipaddr_t uip_all_zeroes_addr = { { 0x0, /* rest is 0 */ } };
#endif
/**
 * Send the answer.
 */
static void sendAnswer(const char const * answer)
{
  PRINTF("I will answer '%s'\n", answer);
  /* Copy the source address in the destination field */
  uip_ipaddr_copy(&remoteconn->ripaddr, &UIP_IP_BUF->srcipaddr);
  /* Send the packet */
  uip_udp_packet_send(remoteconn, answer, strlen(answer));
}

/**
 * Handler of diagnostic request.
 */
static void requestHandler(void)
{
  int i = 0;

  /* Create a "real" C string.*/
  ((char *)uip_appdata)[uip_datalen()] = 0;
  PRINTF("New uIP data: '%s'\n", (char *)uip_appdata);
  /* Search for a potential handler */
  for (i = 0; i < COMMAND_NUM && strcmp(handlers[i]->name, uip_appdata); i++);
  /* If we find one, use it */
  if (i < COMMAND_NUM)
  {
    PRINTF("I chose handler '%s'\n", handlers[i]->name);
    sendAnswer(handlers[i]->handler());
  }
  else
  {
    PRINTF("I don't found the correct handler\n");
#if DIAGNOSTIC_OUTPUT == ASCII_OUTPUT
    sendAnswer("Sorry, I can't find this command...");
#elif DIAGNOSTIC_OUTPUT == XML_OUTPUT
    sendAnswer("<wismote></wismote>");
#elif DIAGNOSTIC_OUTPUT == JSON_OUTPUT
    sendAnswer("{\"wismote\":{}}");
#endif
  }
}

/*---------------------------------------------------------------------------*/
PROCESS(diagnostic_process, "Diagnostic");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(diagnostic_process, ev, data)
{
  PROCESS_BEGIN();

  /* Create the remote and local structures */
  remoteconn = udp_new(&uip_all_zeroes_addr, uip_htons(remote_port), NULL);
  localconn = udp_new(&uip_all_zeroes_addr, 0, NULL);
  /* Listen on the good port */
  udp_bind(localconn,uip_htons(local_port));
  while (1)
  {
    /* Wait for a packet */
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    /* If there is new uIP data, display it */
    if(uip_newdata()) {
      requestHandler();
    }
  }

  PROCESS_END();
}

/** @} **/
