/*
 * Copyright (c) 2011, Plateforme Technologique de Valence.
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

/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2010
 * \version 0.0.3
 *
 * This file defines the "include", "typedef", "define", etc... required by the
 * platform.
 */
#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__


/* For the CC_CONF_* macros, see : /core/sys/cc.h */
/** Support of register keyword. */
#define CC_CONF_REGISTER_ARGS 1
/** Give the keyword for explicitly "inlined" functions. */
#define CC_CONF_INLINE  __inline__
/** Suppress the MACRO CCIF when encountered in code. */
#define CCIF
/** Suppress the MACRO CLIF when encountered in code. */
#define CLIF

/** Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND 128UL

/** Frequency of the MSP430 (in MHz). */
#define MSP430_CLOCK_FREQUENCY 4000000

/** Estimate BR field of UART according to a clock frequency
 * of MSP430_CLOCK_FREQUENCY Hz.*/
#define BAUD2UBR(baudrate) ((unsigned int)(MSP430_CLOCK_FREQUENCY/baudrate))

/** Type for uip_stats. */
typedef unsigned short uip_stats_t;
/** Type for clock_time. */
typedef unsigned long clock_time_t;

#include "msp430.h"
#include <msp430f5437.h>


#endif /* __CONTIKI_CONF_H__ */
