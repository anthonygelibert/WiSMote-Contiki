/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         UDP Server and Client example.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 03, 2011
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

#include "contiki.h"
#include "contiki-net.h"

#include <stdio.h>

#define INTERVAL (CLOCK_CONF_SECOND*10)

static const char remote[] = "192.168.1.1";
static const uint16_t remote_port = 1234;
static const uint16_t local_port = 1235;

/*---------------------------------------------------------------------------*/
PROCESS(ping_process, "Ping Process");
PROCESS(echo_process, "Echo Process");
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
      ((char *)uip_appdata)[uip_datalen()] = 0;
      printf("New uIP data: '%s'\n", uip_appdata);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

/** @} */
