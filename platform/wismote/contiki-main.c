/******************************************************************************
 * Copyright 2010, Plateforme Technologique de Valence                        *
 *                                                                            *
 *    Licensed under the Apache License, Version 2.0 (the "License");         *
 *    you may not use this file except in compliance with the License.        *
 *    You may obtain a copy of the License at                                 *
 *                                                                            *
 *        http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                            *
 *    Unless required by applicable law or agreed to in writing, software     *
 *    distributed under the License is distributed on an "AS IS" BASIS,       *
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *    See the License for the specific language governing permissions and     *
 *    limitations under the License.                                          *
 ******************************************************************************/
/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2010
 * \version 0.0.1
 */

#include "contiki.h"
#include "dev/leds.h"

int
main(void)
{
  /* SETUP : BEGIN */
  /* Initialize the msp430 */
  cpu_init();
  clock_init();
  /* Initialize the leds */
  leds_init();
  leds_on(LEDS_RED);

  /* Initialize the "process system" (core/sys/process.h)     */
  process_init();
  /* SETUP : END */

  autostart_start(autostart_processes);

  while (1) {
    int r;

    do {
      r = process_run();
    }
    while (r > 0);

    /* TODO_PTV: implement idle processing. */
  }
  return 0;
}

