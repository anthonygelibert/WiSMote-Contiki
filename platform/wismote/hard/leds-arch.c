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
 * \author Anthony Gelibert
 * \date Jan 24, 2010
 * \version 0.0.2
 */

#include "dev/leds.h"

/** PxDIR used for the blue LED. */
#define PORTDIR_LED_BLUE  P2DIR
/** PxDS used for the blue LED. */
#define PORTDS_LED_BLUE   P2DS
/** PxOUT used for the blue LED. */
#define PORTOUT_LED_BLUE  P2OUT
/** BIT used for the blue LED. */
#define BIT_LED_BLUE      BIT2

/** PxDIR used for the red LED. */
#define PORTDIR_LED_RED   P5DIR
/** PxDS used for the red LED. */
#define PORTDS_LED_RED    P5DS
/** PxOUT used for the red LED. */
#define PORTOUT_LED_RED   P5OUT
/** BIT used for the red LED. */
#define BIT_LED_RED       BIT4

/** PxDIR used for the green LED. */
#define PORTDIR_LED_GREEN P8DIR
/** PxDS used for the green LED. */
#define PORTDS_LED_GREEN  P8DS
/** PxOUT used for the green LED. */
#define PORTOUT_LED_GREEN P8OUT
/** BIT used for the green LED. */
#define BIT_LED_GREEN     BIT6

/** Initialize the ports used by the LEDs. */
void leds_arch_init(void)
{
    PORTDIR_LED_GREEN |= BIT_LED_GREEN;
    PORTDIR_LED_RED   |= BIT_LED_RED;
    PORTDIR_LED_BLUE  |= BIT_LED_BLUE;

    PORTDS_LED_GREEN  |= BIT_LED_GREEN;
    PORTDS_LED_RED    |= BIT_LED_RED;
    PORTDS_LED_BLUE   |= BIT_LED_BLUE;
}

/**
 * Get the state of the leds.
 *
 * \return NOT_YET_DOCUMENTED_PTV
 *
 */
unsigned char leds_arch_get(void)
{
    return ((PORTOUT_LED_RED & BIT_LED_RED) ? 0 : LEDS_RED)
      | ((PORTOUT_LED_GREEN & BIT_LED_GREEN) ? 0 : LEDS_GREEN)
      | ((PORTOUT_LED_BLUE & BIT_LED_BLUE) ? 0 : LEDS_BLUE);
}

/**
 * Set the state of the leds.
 *
 * \param leds NOT_YET_DOCUMENTED_PTV
 */
void leds_arch_set(unsigned char leds)
{
    PORTOUT_LED_RED = (PORTOUT_LED_RED & ~BIT_LED_RED) | ((leds & LEDS_RED) ? 0 : BIT_LED_RED);
    PORTOUT_LED_GREEN = (PORTOUT_LED_GREEN & ~BIT_LED_GREEN) | ((leds & LEDS_GREEN) ? 0 : BIT_LED_GREEN);
    PORTOUT_LED_BLUE = (PORTOUT_LED_BLUE & ~BIT_LED_BLUE) | ((leds & LEDS_BLUE) ? 0 : BIT_LED_BLUE);
}
