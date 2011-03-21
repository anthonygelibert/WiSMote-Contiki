/**
 * \file
 *         Parallax PIR 555-28027.
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

#include <signal.h>

#include "parallax_pir-555-28027.h"
#include "iohandlers.h"

const struct sensors_sensor PIR_555_28027_sensor;

static void myHandler(void);

HWCONF_PIN(PRESENCE, 1, 2)
HWCONF_IRQ(PRESENCE, 1, 2, myHandler)

/*---------------------------------------------------------------------------*/

static void
myHandler(void)
{
  /* Change the internal resistance state
   * (pull-up -> pull-down // pull-down -> pull-up)
   */
  PRESENCE_RESISTOR_PULL_SWAP();
  /*
   * Change the level detection
   * (up -> down // down up)
   */
  PRESENCE_IRQ_EDGE_SWAP();
  /* Notify the sensor module */
  sensors_changed(&PIR_555_28027_sensor);
}

/*---------------------------------------------------------------------------*/

static int
value(int type)
{
  return PRESENCE_READ() ;
}

/*---------------------------------------------------------------------------*/

static int
configure(int type, int c)
{
  switch (type) {
    case SENSORS_ACTIVE:
      if (c) {
        PRESENCE_RESISTOR_ENABLE();
        PRESENCE_RESISTOR_PULL_DOWN();
        PRESENCE_IRQ_EDGE_SELECTU();
        PRESENCE_SELECT_IO();
        PRESENCE_MAKE_INPUT();
        PRESENCE_SET_HANDLER();
        PRESENCE_ENABLE_IRQ();
      } else {
        PRESENCE_DISABLE_IRQ();
        PRESENCE_RESET_HANDLER();
        PRESENCE_RESISTOR_ENABLE();
        PRESENCE_RESISTOR_PULL_DOWN();
      }
      return 1;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

static int
status(int type)
{
  switch (type) {
    case SENSORS_ACTIVE:
    case SENSORS_READY:
      return PRESENCE_IRQ_ENABLED();
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

SENSORS_SENSOR(PIR_555_28027_sensor, PIR_555_28027_SENSOR, value, configure, status);

/** @} */
