/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: msp430.h,v 1.4 2011/01/05 13:36:38 joxe Exp $
 */

#ifndef __MSP430_H__
#define __MSP430_H__

#ifndef C_MSP430_H__
#define PUBLIC extern
#else
#define PUBLIC
#endif

#ifdef F_CPU
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_CPU_SPEED F_CPU
#else
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_CPU_SPEED 2457600UL
#endif

/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_CPUON 0
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM1 1
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM2 2
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM3 3

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * \note We add the LPM4 level... which was not present in sky.
 */
#define MSP430_REQUIRE_LPM4 4

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @param req NOT_YET_DOCUMENTED_PTV
 */
PUBLIC void msp430_add_lpm_req(int req);
PUBLIC void msp430_remove_lpm_req(int req);

#endif /* __MSP430_H__ */
