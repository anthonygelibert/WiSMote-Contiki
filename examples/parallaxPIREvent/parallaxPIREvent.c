/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Parallax PIR sensor with event example.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 *         Fabien Rey <fabien-rey@wanadoo.fr>
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

#include "contiki.h"
#include "lib/sensors.h"
#include "parallax_pir-555-28027.h"
#include <stdio.h>


/*---------------------------------------------------------------------------*/
PROCESS(parallaxPIREvent_process, "Parallax PIR Event process");
AUTOSTART_PROCESSES(&parallaxPIREvent_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(parallaxPIREvent_process, ev, data)
{
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(PIR_555_28027_sensor);
  while (1)
  {
    /* I wait for a specific situation:
     *   - The event is of kind "sensor".
     *   - The concerned sensor is "PIR 555-28027".
     */
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &PIR_555_28027_sensor);
    /* I use the pointer to the sensor (data) to interrogate it.*/
    int presence = ((struct sensors_sensor *)data)->value(0);
    if (presence)
    {
      printf("Someone is here !!!\n");
    }
    else
    {
      printf("Nobody\n");
    }
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

/** @} */
