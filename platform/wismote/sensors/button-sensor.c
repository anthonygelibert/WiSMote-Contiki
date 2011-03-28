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

/* From MPS430-GCC */
#include <signal.h>

/* From CONTIKI*/
#include "dev/button-sensor.h"
#include "lib/sensors.h"

/* From MSP430x5xx */
#include "iohandlers.h"

const struct sensors_sensor button_sensor;
static struct timer debouncetimer;

/*---------------------------------------------------------------------------*/

/**
 * Change the status of the sensor only if the "de-bounce" timer expired.
 */
static void
myHandler(void)
{
  if (timer_expired(&debouncetimer)) {
    timer_set(&debouncetimer, CLOCK_SECOND / 4);
    sensors_changed(&button_sensor);
    LPM4_EXIT;
  }
}

/*---------------------------------------------------------------------------*/
/* See iohandlers.h.
 * These macros define the BUTTON_* functions according to the specified
 * port, bit and IT handler.
 */
HWCONF_PIN(BUTTON, 1, 4)
HWCONF_IRQ(BUTTON, 1, 4, myHandler)

/*---------------------------------------------------------------------------*/

/**
 * Read the value of the button.
 */
static int
value(int type)
{
  return BUTTON_READ() || !timer_expired(&debouncetimer);
}

/*---------------------------------------------------------------------------*/

/**
 * Enable/Disable the sensor.
 *
 * @param type Configuration type.
 * @param c If type == SENSORS_ACTIVE, 0 ~> Disable / ~0 ~> Enable.
 *
 * @retval 1 Operation done.
 * @retval 0 No operation done.
 */
static int
configure(int type, int c)
{
  switch (type) {
    case SENSORS_ACTIVE:
      if (c) {
        timer_set(&debouncetimer, 0);
        BUTTON_IRQ_EDGE_SELECTD();
        BUTTON_SELECT_IO();
        BUTTON_MAKE_INPUT();
        BUTTON_SET_HANDLER();
        BUTTON_ENABLE_IRQ();
      } else {
        BUTTON_DISABLE_IRQ();
        BUTTON_RESET_HANDLER();
        BUTTON_MAKE_OUTPUT();
        timer_set(&debouncetimer, 0);
      }
      return 1;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

/** Use the IRQ state to determine the curent status */
static int
status(int type)
{
  switch (type) {
    case SENSORS_ACTIVE:
    case SENSORS_READY:
      return BUTTON_IRQ_ENABLED();
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

SENSORS_SENSOR(button_sensor, BUTTON_SENSOR, value, configure, status);

/** @} */
