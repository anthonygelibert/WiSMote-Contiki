/**
 * \file
 *         TCP Server template.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 17, 2011
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

#include "contiki.h"
#include "contiki-net.h"

#include <stdio.h>

static uint16_t local_port = 1234;

/*---------------------------------------------------------------------------*/
PROCESS(tcpEcho_process, "TCP Echo process");
AUTOSTART_PROCESSES(&tcpEcho_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tcpEcho_process, ev, data)
{

  PROCESS_BEGIN();

  /* Listen for a connection on local port */
  tcp_listen(UIP_HTONS(local_port));
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    /* If there is new uIP data, display it */
    if(uip_newdata()) {
      /* Add the end of string. */
      ((char *)uip_appdata)[uip_datalen()] = 0;
      printf("New uIP data: '%s'\n", uip_appdata);
    }
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
