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
 * \version 0.0.1
 */

#include "contiki-conf.h"
#define __C_DCO_H__
#include "dco.h"

/** dco_required set to 1 will cause the CPU not to go into sleep modes where the DCO clock stopped */
static int msp430_dco_required;

#ifdef F_CPU
/** CPU speed of MSP430 (in MHz) (F_CPU macro). */
#define MSP430_CPU_SPEED F_CPU
#else
/** CPU speed of MSP430 (in MHz) (F_CPU macro). */
#define MSP430_CPU_SPEED 2457600UL
#endif

/* default DCOSYNCH Period is 30 seconds */
#ifdef DCOSYNCH_CONF_PERIOD
/** NOT_YET_DOCUMENTED_PTV */
#define DCOSYNCH_PERIOD DCOSYNCH_CONF_PERIOD
#else
/** NOT_YET_DOCUMENTED_PTV */
#define DCOSYNCH_PERIOD 30
#endif

/** Initialize DCO
 */
void
msp430_init_dco(void)
{
  P4DIR |= 0x80;
  P4SEL |= 0x80;

  // Activate XT1
  P7SEL |= BIT0 + BIT1;
  // Set XT1 On
  UCSCTL6 &= ~XT1OFF;
  // Max drive strength, adjust according to crystal frequency. LFXT1 HF mode
  UCSCTL6 |= XT1DRIVE_2 + XTS + XT2OFF;

  do
    {
      UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                              // Clear XT2,XT1,DCO fault flags
      SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL4 = SELA_0 + SELS_0 + SELM_0;       // MCLCK = XT1 , SMCLK = XT1 , ACLK = XT1
  UCSCTL5 |= DIVA__4 + DIVS__4 + DIVM__4;// Set Clock divider 4 -> Clock = 4MHz


}

/** Increment the msp430_dco_required value. */
void
msp430_dco_required_inc(void)
{
  msp430_dco_required++;
}

/** Decrement the msp430_dco_required value. */
void
msp430_dco_required_dec(void)
{
  msp430_dco_required--;
}

/** Reset the msp430_dco_required value. */
void
msp430_dco_required_reset(void)
{
  msp430_dco_required = 0;
}
