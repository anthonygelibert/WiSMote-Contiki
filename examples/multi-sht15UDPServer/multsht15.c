/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Multiple SHT15 Sensor using UDP Server example.
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

#include <stdint.h>
#include <string.h>

#include "contiki.h"
#include "contiki-net.h"
#include "lib/sensors.h"
#include "iohandlers.h"
#include "watchdog.h"
#include "dev/ds2411.h"

#include "multsht15-sensors.h"
#include "sensors/sht1x-sensor.h"

/** Enable debug output. */
#define DEBUG 1

#if DEBUG
#include <stdio.h>
/** PRINTF is a real printf. */
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define ARRAY_SIZE 80

/** The uIP buffer. */
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
/** The local port. */
static const uint16_t local_port = 1234;
/** The remote port. */
static const uint16_t remote_port = 1235;
/** The local connection. */
static struct uip_udp_conn *localconn;
/** The remote connection. */
static struct uip_udp_conn *remoteconn;

static void getSensor(const int num, char *buf)
{
  unsigned int rh;
  unsigned int tmp;
  unsigned int pwr;
  struct sensors_sensor * sensor = getSensorByNum(num);

  snprintf(buf,ARRAY_SIZE,"%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X::",
      ds2411_id[0], ds2411_id[1], ds2411_id[2], ds2411_id[3],
      ds2411_id[4], ds2411_id[5], ds2411_id[6], ds2411_id[7]);
  strncat(buf,sensor->type,ARRAY_SIZE);
  SENSORS_ACTIVATE(*sensor);
  tmp = sensor->value(SHT1X_SENSOR_TEMP);
  rh = sensor->value(SHT1X_SENSOR_HUMIDITY);
  pwr = sensor->value(SHT1X_SENSOR_BATTERY_INDICATOR);
  SENSORS_DEACTIVATE(*sensor);
  snprintf(buf,ARRAY_SIZE,"%s::%d::%d::%d\n",buf,tmp,rh,pwr);
}

/*---------------------------------------------------------------------------*/
PROCESS(server_process, "Mult-SHT15_Server");
AUTOSTART_PROCESSES(&server_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(server_process, ev, data)
{
  static char buffer[ARRAY_SIZE];
  int sensorNum;

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
        ((char *)uip_appdata)[uip_datalen()] = 0;
        PRINTF("New uIP data: '%s'\n", (char *)uip_appdata);
        uip_ipaddr_copy(&remoteconn->ripaddr, &UIP_IP_BUF->srcipaddr);
        memset(buffer,0x0,ARRAY_SIZE);
        sensorNum = *((char *)uip_appdata) - 0x30;
        getSensor(sensorNum, buffer);
        uip_udp_packet_send(remoteconn, buffer, strlen(buffer));
    }
  }

  PROCESS_END();
}
/** @} */
