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

#include <io.h>
#include <signal.h>
#include <sys/unistd.h>

#define C_MSP430_H__
#include "msp430.h"

#include "msp430def.h"
#include "dev/watchdog.h"
#include "net/uip.h"

#pragma mark DCO

/** dco_required set to 1 will cause the CPU not to go into sleep modes where the DCO clock stopped */
int msp430_dco_required;

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

#pragma mark port

/** Ports initialization.
 *
 * Initialize all the ports of the platform:
 * <ul>
 *    <li>PxSEL = 0</li>
 *    <li>PxDIR = 0</li>
 *    <li>PxOUT = 0</li>
 *    <li>PxIE  = 0</li>
 * </ul>
 */
static void init_ports(void)
{
    /* All configured for digital I/O */
#ifdef P1SEL
    P1SEL = 0;
#endif
#ifdef P2SEL
    P2SEL = 0;
#endif
#ifdef P3SEL
    P3SEL = 0;
#endif
#ifdef P4SEL
    P4SEL = 0;
#endif
#ifdef P5SEL
    P5SEL = 0;
#endif
#ifdef P6SEL
    P6SEL = 0;
#endif
#ifdef P7SEL
    P7SEL = 0;
#endif
#ifdef P8SEL
    P8SEL = 0;
#endif

    /* All available inputs */
#ifdef P1DIR
    P1DIR = 0;
    P1OUT = 0;
#endif
#ifdef P2DIR
    P2DIR = 0;
    P2OUT = 0;
#endif
#ifdef P3DIR
    P3DIR = 0;
    P3OUT = 0;
#endif
#ifdef P4DIR
    P4DIR = 0;
    P4OUT = 0;
#endif
#ifdef P5DIR
    P5DIR = 0;
    P5OUT = 0;
#endif
#ifdef P6DIR
    P6DIR = 0;
    P6OUT = 0;
#endif
#ifdef P7DIR
    P7DIR = 0;
    P7OUT = 0;
#endif
#ifdef P8DIR
    P8DIR = 0;
    P8OUT = 0;
#endif

    /* All available IE */
#ifdef P1IE
    P1IE = 0;
#endif
#ifdef P2IE
    P2IE = 0;
#endif
}


/*---------------------------------------------------------------------------*/
/* msp430-ld may align _end incorrectly. Workaround in cpu_init. */
extern int _end; /* Not in sys/unistd.h */
static char *cur_break = (char *) &_end;

/*---------------------------------------------------------------------------*/
/* add/remove_lpm_req - for requiring a specific LPM mode. currently Contiki */
/* jumps to LPM3 to save power, but DMA will not work if DCO is not clocked  */
/* so some modules might need to enter their LPM requirements                */
/* NOTE: currently only works with LPM1 (e.g. DCO) requirements.             */
/*---------------------------------------------------------------------------*/
void msp430_add_lpm_req(int req)
{
    if (req <= MSP430_REQUIRE_LPM1)
    {
        msp430_dco_required++;
    }
}

void msp430_remove_lpm_req(int req)
{
    if (req <= MSP430_REQUIRE_LPM1)
    {
        msp430_dco_required--;
    }
}

void msp430_cpu_init(void)
{
    dint();
    watchdog_init();
    init_ports();

    msp430_init_dco();

    eint();
    if ((uintptr_t) cur_break & 1)
    { /* Workaround for msp430-ld bug! */
        cur_break++;
    }
    msp430_dco_required = 0;
}
/*---------------------------------------------------------------------------*/
#define asmv(arg) __asm__ __volatile__(arg)

#define STACK_EXTRA 32

/*
 * Allocate memory from the heap. Check that we don't collide with the
 * stack right now (some other routine might later). A watchdog might
 * be used to check if cur_break and the stack pointer meet during
 * runtime.
 */
void *
sbrk(int incr)
{
    char *stack_pointer;

    asmv("mov r1, %0" : "=r" (stack_pointer));
    stack_pointer -= STACK_EXTRA;
    if (incr > (stack_pointer - cur_break))
        return (void *) -1; /* ENOMEM */

    void *old_break = cur_break;
    cur_break += incr;
    /*
     * If the stack was never here then [old_break .. cur_break] should
     * be filled with zeros.
     */
    return old_break;
}
/*---------------------------------------------------------------------------*/
/*
 * Mask all interrupts that can be masked.
 */
int splhigh_(void)
{
    /* Clear the GIE (General Interrupt Enable) flag. */
    int sr;
    asmv("mov r2, %0" : "=r" (sr));
    asmv("bic %0, r2" : : "i" (GIE));
    return sr & GIE; /* Ignore other sr bits. */
}
/*---------------------------------------------------------------------------*/
/*
 * Restore previous interrupt mask.
 */
void splx_(int sr)
{
    /* If GIE was set, restore it. */
    asmv("bis %0, r2" : : "r" (sr));
}

