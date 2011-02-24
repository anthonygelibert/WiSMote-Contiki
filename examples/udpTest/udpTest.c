/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: hello-world.c,v 1.1 2006/10/02 21:46:46 adamdunkels Exp $
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h> /* For printf() */

static char server[] = "192.168.1.1";
static uint16_t port = 1234;

/*---------------------------------------------------------------------------*/
PROCESS(ping_process, "Ping process");
AUTOSTART_PROCESSES(&ping_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ping_process, ev, data)
{
  static uip_ipaddr_t serveraddr;
  static struct uip_udp_conn *udpconn;
  static struct etimer et;

  PROCESS_BEGIN();

  uiplib_ipaddrconv(server, &serveraddr);
  udpconn = udp_new(&serveraddr, uip_htons(port), NULL);
  printf("IP: %u.%u.%u.%u\nLP: %u\nRP: %u\nTTL: %u\n",
      (udpconn->ripaddr).u8[0],
      (udpconn->ripaddr).u8[1],
      (udpconn->ripaddr).u8[2],
      (udpconn->ripaddr).u8[3],
      udpconn->lport,
      udpconn->rport,
      udpconn->ttl);
  while (1)
  {
    etimer_set(&et, CLOCK_CONF_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    uip_udp_packet_send(udpconn, "Hello world !!!", 16);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
