/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Clock platform-dependent code.
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
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
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

/* From CONTIKI */
#include <contiki-conf.h>
/* From MSP430-GCC */
#include "clock-arch.h"

#ifndef SMCLOCK_CONF_DEBUG
#define DEBUG_SMCLOCK 0
#else
#define DEBUG_SMCLOCK (SMCLOCK_CONF_DEBUG)
#endif

/*---------------------------------------------------------------------------*/

/**
 * \brief Initialize the clock for our platform.
 *
 * Set the port for the XT1 clock and if DEBUG_CLOCK is defined, redirect the
 * SMCLOCK on a port of the platform.
 */
void clock_platform_init(void)
{
#if DEBUG_SMCLOCK
  P4DIR |= 0x80;
  P4SEL |= 0x80;
#endif
  /* XT1 */
  P7SEL |= BIT0 + BIT1;
}

/*---------------------------------------------------------------------------*/

/** @} */
