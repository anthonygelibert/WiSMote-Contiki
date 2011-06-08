/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Multiple SHT15 Sensor example.
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
#include <stdio.h>

#include "contiki.h"
#include "lib/sensors.h"
#include "iohandlers.h"

#include "multsht15-sensors.h"
#include "sensors/sht1x-sensor.h"

#define INTERVAL (CLOCK_CONF_SECOND * 5)

static void printSensor(const struct sensors_sensor sensor)
{
  static unsigned int rh;
  static unsigned int tmp;
  static unsigned int pwr;

  /* Enable the sensor. */
  SENSORS_ACTIVATE(sensor);
  /* Read the temperature. */
  tmp = sensor.value(SHT1X_SENSOR_TEMP);
  if (tmp == -1)
  {
    printf("TMP: N/A\n");
  }
  else
  {
    printf("TMP: %d\n", tmp);
  }

  /* Read the humidity */
  rh = sensor.value(SHT1X_SENSOR_HUMIDITY);
  if (rh == -1)
  {
    printf("HR:  N/A\n");
  }
  else
  {
    printf("HR:  %d\n", rh);
  }
  /* Check the battery warning */

  pwr = sensor.value(SHT1X_SENSOR_BATTERY_INDICATOR);
  if (pwr == -1)
  {
      printf("PWR: N/A\n");
  }
  else
  {
      printf("PWR: %d\n", pwr);
  }
  /* Disable the sensor */
  SENSORS_DEACTIVATE(sensor);
}

/*---------------------------------------------------------------------------*/
PROCESS(multsht15_process, "SHT15 process");
AUTOSTART_PROCESSES(&multsht15_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(multsht15_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  while (1)
  {
    /* Check temperature every INTERVAL */
    etimer_set(&et, INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    printSensor(upper_sensor);
    printSensor(lower_sensor);
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

/** @} */
