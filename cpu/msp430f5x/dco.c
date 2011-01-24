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
 *
 * FIXME_PTV
 */
void msp430_init_dco(void)
{
    /* This code taken from the FU Berlin sources and reformatted. */
#define DELTA    ((MSP430_CPU_SPEED) / (32768 / 8))

    unsigned int compare, oldcapture = 0;
    unsigned int i;

    /* ACLK is devided by 4. RSEL=6 no division for MCLK and SSMCLK. XT2 is off. */
    BCSCTL1 = 0xa4;

    /* Init FLL to desired frequency using the 32762Hz crystal DCO frquenzy = 2,4576 MHz  */
    BCSCTL2 = 0x00;

    BCSCTL1 |= DIVA1 + DIVA0; /* ACLK = LFXT1CLK/8 */
    for (i = 0xffff; i > 0; i--)
    { /* Delay for XTAL to settle */
        asm("nop");
    }

    CCTL2 = CCIS0 + CM0 + CAP; // Define CCR2, CAP, ACLK
    TACTL = TASSEL1 + TACLR + MC1; // SMCLK, continous mode


    while (1)
    {

        while ((CCTL2 & CCIFG) != CCIFG)
            ; /* Wait until capture occured! */
        CCTL2 &= ~CCIFG; /* Capture occured, clear flag */
        compare = CCR2; /* Get current captured SMCLK */
        compare = compare - oldcapture; /* SMCLK difference */
        oldcapture = CCR2; /* Save current captured SMCLK */

        if (DELTA == compare)
        {
            break; /* if equal, leave "while(1)" */
        }
        else
            if (DELTA < compare)
            { /* DCO is too fast, slow it down */
                DCOCTL--;
                if (DCOCTL == 0xFF)
                { /* Did DCO role under? */
                    BCSCTL1--;
                }
            }
            else
            { /* -> Select next lower RSEL */
                DCOCTL++;
                if (DCOCTL == 0x00)
                { /* Did DCO role over? */
                    BCSCTL1++;
                }
                /* -> Select next higher RSEL  */
            }
    }

    CCTL2 = 0; /* Stop CCR2 function */
    TACTL = 0; /* Stop Timer_A */

    BCSCTL1 &= ~(DIVA1 + DIVA0); /* remove /8 divisor from ACLK again */
}

/** Synchronize DCO.
 *
 *  \note this code will always start the TimerB if not already started
 *  FIXME_PTV
 */
void msp430_sync_dco(void)
{
    uint16_t last;
    uint16_t diff;
    /*   uint32_t speed; */
    /* DELTA_2 assumes an ACLK of 32768 Hz */
#define DELTA_2    ((MSP430_CPU_SPEED) / 32768)

    /* Select SMCLK clock, and capture on ACLK for TBCCR6 */
    TBCTL = TBSSEL1 | TBCLR;
    TBCCTL6 = CCIS0 + CM0 + CAP;
    /* start the timer */
    TBCTL |= MC1;

    // wait for next Capture
    TBCCTL6 &= ~CCIFG;
    while (!(TBCCTL6 & CCIFG))
        ;
    last = TBCCR6;

    TBCCTL6 &= ~CCIFG;
    // wait for next Capture - and calculate difference
    while (!(TBCCTL6 & CCIFG))
        ;
    diff = TBCCR6 - last;

    /* Stop timer - conserves energy according to user guide */
    TBCTL = 0;

    /*   speed = diff; */
    /*   speed = speed * 32768; */
    /*   printf("Last TAR diff:%d target: %ld ", diff, DELTA_2); */
    /*   printf("CPU Speed: %lu DCOCTL: %d\n", speed, DCOCTL); */

    /* resynchronize the DCO speed if not at target */
    if (DELTA_2 < diff)
    { /* DCO is too fast, slow it down */
        DCOCTL--;
        if (DCOCTL == 0xFF)
        { /* Did DCO role under? */
            BCSCTL1--;
        }
    }
    else
        if (DELTA_2 > diff)
        {
            DCOCTL++;
            if (DCOCTL == 0x00)
            { /* Did DCO role over? */
                BCSCTL1++;
            }
        }
}

/** NOT_YET_DOCUMENTED_PTV */
void msp430_dco_required_inc(void)
{
    msp430_dco_required++;
}

/** NOT_YET_DOCUMENTED_PTV */
void msp430_dco_required_dec(void)
{
    msp430_dco_required--;
}

/** NOT_YET_DOCUMENTED_PTV */
void msp430_dco_required_reset(void)
{
    msp430_dco_required = 0;
}
