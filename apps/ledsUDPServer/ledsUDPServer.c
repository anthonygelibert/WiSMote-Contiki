/**
 * \file
 *         LEDs UDP Server.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 07, 2011
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
#include "dev/leds.h"

#include <string.h>
#include <stdio.h>

#define DEBUG 1

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/** Local port used by the server. */
static const uint16_t local_port = 1234;
/** Prefix of a "LED Toggling" packet. */
static const char preamble[] = "LED-";
/** Size of the preamble. */
static const uint8_t preambleSize = 4;

/*---------------------------------------------------------------------------*/
PROCESS(ledsUDPServer_process, "LEDS UDP Server Process");
AUTOSTART_PROCESSES(&ledsUDPServer_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ledsUDPServer_process, ev, data)
{
  static struct uip_udp_conn *localconn;

  PROCESS_BEGIN();

  /* Create a local conn. structure :
   *  - Remote IP: 0.0.0.0 (all accepted).
   *  - Remote port: 0 (all accepted).
   */
  localconn = udp_new(&uip_all_zeroes_addr, 0, NULL);
  /* Bind to the local port. */
  udp_bind(localconn,uip_htons(local_port));
  PRINTF("I listen the UDP port %u\n",uip_ntohs(localconn->lport));
  while (1)
  {
    PRINTF("I wait a new request\n");
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    /* Here, a new TCP/IP event occurred ! */
    /* A good packet is :
     *  - With data.
     *  - With at least "preambleSize + 1" bytes.
     *  - Packet start with the good preamble.
     */
    if(uip_newdata() && uip_datalen() >= (preambleSize + 1) &&
       !strncmp(uip_appdata,preamble,preambleSize))
    {
      PRINTF("I received a LED toggling request.\n");
      {
        char *data = (char *)uip_appdata;
        int led = 0;
        switch (data[4]) {
          case '1': led = LEDS_BLUE;  break;
          case '2': led = LEDS_GREEN; break;
          case '3': led = LEDS_RED;   break;
          default : PRINTF("I can't find the good LED...\n"); continue;
        }
        leds_toggle(led);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
