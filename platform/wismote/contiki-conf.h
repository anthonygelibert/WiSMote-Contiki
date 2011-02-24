/**
 * \defgroup wismote Wismote platform
 *
 * This is the module implementing Contiki on the Wismote platform.
 *
 * @{
 */

/**
 * \file
 *         Contiki configuration.
 * \author
 *         Anthony Gelibert <anthony.gelibert@me.com>
 * \date
 *         Feb 18, 2011
 */

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

#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

/* ----- Compiler macros ----- */
/* For the CC_CONF_* macros, see : /core/sys/cc.h */
#define CC_CONF_REGISTER_ARGS 1
#define CC_CONF_INLINE  __inline__
#define CCIF
#define CLIF

/* ----- Clock module ----- */
#define CLOCK_CONF_SECOND 128UL
typedef unsigned long clock_time_t;

/* ----- UIP module ----- */
typedef unsigned short uip_stats_t;
#define UIP_CONF_LOGGING           1
#define UIP_CONF_UDP               1
#define UIP_CONF_TCP               1
#define UIP_CONF_ICMP_DEST_UNREACH 1
#define UIP_CONF_MAX_CONNECTIONS   4
#define UIP_CONF_MAX_LISTENPORTS   8
#define UIP_CONF_UDP_CONNS         12
#define UIP_CONF_FWCACHE_SIZE      30
#define UIP_CONF_BROADCAST         1
#define UIP_CONF_PINGADDRCONF      0
#define UIP_CONF_TCP_SPLIT         0

/* ----- UART module ----- */
#define UART0_CONF_TX_WITH_INTERRUPT 1
#define UART0_CONF_TX_BUFSIZE 64

/* ----- Serial Line module ----- */
#define SERIAL_LINE_CONF_BUFSIZE 64

/* ----- Includes ----- */
#include "types.h"
#include <msp430f5437.h>

#endif /* __CONTIKI_CONF_H__ */

/** @} */

