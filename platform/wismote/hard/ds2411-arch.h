/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         DS2411 platform-dependent code.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         May 23, 2011
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

#ifndef __DS2411_ARCH_H__
#define __DS2411_ARCH_H__

/** PIN used by the 1-wire. */
#define PIN BIT1

/** Initialize the port 1. */
#define PIN_INIT() {\
  P1REN |= PIN;  /* Enable internal pull-up */ \
  P1DIR &= ~PIN; /* Input */ \
}

/** Set 1-Wire to low. */
#define OUTP_0() P1DIR |=  PIN;\
                 P1OUT &= ~PIN;
/** Set 1-wire to high. */
#define OUTP_1() P1DIR |= PIN;\
                 P1OUT |= PIN;\
                 P1DIR &= ~PIN;

/** Read one bit. */
#define INP()    (P1IN & PIN)

/**
 * Delay for u microseconds on a MSP430 at 4MHz.
 */
#define udelay(u) clock_delay((u*8 - 14)/6)

#define udelay_6() { _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); }

#endif /* __DS2411_ARCH_H__ */
