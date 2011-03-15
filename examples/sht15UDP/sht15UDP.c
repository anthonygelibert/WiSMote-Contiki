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
#include "dev/sht11-sensor.h"

#include <stdio.h>

#define INTERVAL (CLOCK_CONF_SECOND*10)

static const char remote[] = "172.28.1.224";
static const uint16_t remote_port = 1234;

/*---------------------------------------------------------------------------*/
PROCESS(ping_process, "Ping Process");
AUTOSTART_PROCESSES(&ping_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ping_process, ev, data)
{
  static uip_ipaddr_t serveraddr;
  static struct uip_udp_conn *udpconn;
  static struct etimer et;
  static unsigned int tmp;
  static unsigned int rh;
  static char string[10];

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(sht11_sensor);


  uiplib_ipaddrconv(remote, &serveraddr);
  udpconn = udp_new(&serveraddr, uip_htons(remote_port), NULL);
  while (1)
  {
    etimer_set(&et, INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    tmp = sht11_sensor.value(SHT11_SENSOR_TEMP);
    rh = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
    sprintf(string,"%4d-%4d",(tmp==-1?0:tmp),(rh==-1?0:rh));
    uip_udp_packet_send(udpconn, string, 10);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

/** @} */
