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
/* XXX_PTV Change to long */
typedef unsigned short clock_time_t;

/* ----- Includes ----- */
#include "types.h"
#include <msp430f5437.h>
#if UIP_USE_DS2411_FOR_MAC_ADDRESS
#include "ds2411-arch.h"
#endif
#include "spl.h"

/* ----- UIP module ----- */
/* ---- Stack ----- */
#if WITH_UIP6
#ifndef NETSTACK_CONF_NETWORK
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#endif
#else
#ifndef NETSTACK_CONF_NETWORK
#define NETSTACK_CONF_NETWORK rime_driver
#endif
#endif
#ifndef NETSTACK_CONF_RADIO
#define NETSTACK_CONF_RADIO   cc2520_driver
#endif

/* ---- Perso ----- */
#define UIP_USE_DS2411_FOR_MAC_ADDRESS 0
/* ---- General ---- */
typedef unsigned short uip_stats_t;
/** Globally enable logging. */
#define PLATFORM_LOGGING           0
#if PLATFORM_LOGGING
/** Enable UIP logging. */
#define UIP_CONF_LOGGING           1
/** Enable configuration logging. */
#define LOG_CONF_ENABLED           1
/** Enable UIP statistics. */
#define UIP_CONF_STATISTICS        1
#else
/** Disable UIP logging. */
#define UIP_CONF_LOGGING           0
/** Disable configuration logging. */
#define LOG_CONF_ENABLED           0
/** Disable UIP statistics. */
#define UIP_CONF_STATISTICS        0
#endif
/** Disable PING ADDR. CONF. */
#define UIP_CONF_PINGADDRCONF      0
/** Disable IP packet reassembly. */
#define UIP_CONF_REASSEMBLY        0
/* ---- UDP ---- */
/** Enable UDP compilation. */
#define UIP_CONF_UDP               1
/** Disable UDP checksum. */
#define UIP_CONF_UDP_CHECKSUMS     0
#if UIP_CONF_UDP
/** Number of simultaneous connection. */
#define UIP_CONF_UDP_CONNS         5
/** Enable broadcast. */
#define UIP_CONF_BROADCAST         1
#else
#define UIP_CONF_UDP_CONNS         0
/** Disable UDP broadcast. */
#define UIP_CONF_BROADCAST         0
#endif

/* ---- TCP ---- */
/** Enable TCP compilation. */
#define UIP_CONF_TCP               1
#if UIP_CONF_TCP
/** Enable open connection. */
#define UIP_CONF_ACTIVE_OPEN       1
/** Number of open connection. */
#define UIP_CONF_MAX_CONNECTIONS   5
/** Number of open ports. */
#define UIP_CONF_MAX_LISTENPORTS   5
#else
/** Disable open connex. */
#define UIP_CONF_ACTIVE_OPEN       0
/** No open connex. */
#define UIP_CONF_MAX_CONNECTIONS   0
/** No port available. */
#define UIP_CONF_MAX_LISTENPORTS   0
#endif

/* ----- SPI module ----- */
/** SPI: TX Buffer. */
#define SPI_TXBUF UCB0TXBUF
/** SPI: RX Buffer. */
#define SPI_RXBUF UCB0RXBUF
/** USART0 Tx ready? */
#define SPI_WAITFOREOTx() while ((UCB0STAT & UCBUSY) != 0)
/** USART0 Rx ready? */
#define SPI_WAITFOREORx() while ((UCB0IFG & UCRXIFG) == 0)
/** USART0 Tx buffer ready? */
#define SPI_WAITFORTxREADY() while ((UCB0IFG & UCTXIFG) == 0)

/* ----- CLOCK module ----- */
/** CLOCK: this platform uses XT1 clock. */
#define XT1_CONF_CLOCK 1
/** CLOCK: disable debug. */
#define SMCLOCK_CONF_DEBUG 0

/* ----- IOHANDLERS module ----- */
/** IOHANDLERS: disable debug. */
#define IOHANDLERS_CONF_DEBUG 0

/* ----- UART choice ----- */
/** UART1: SLIP doesn't use the MAIN_UART */
#define SLIP_USE_UART1  0
/** UART1: "stdio.h" uses the MAIN_UART */
#define STDIO_USE_UART1 0
/** Serial Line module uses the MAIN_UART */
#define SL_USE_UART1    0

/* ----- UART module ----- */
/** Use simple RX/TX indicator. */
#define UART0_CONF_RXTX_INDICATOR_SIMPLE 1
/** Doesn't use complex RX/TX indicator. */
#define UART0_CONF_RXTX_INDICATOR_COMPLEX 0
/** UART0: uses interrupt for TX. */
#define UART0_CONF_TX_WITH_INTERRUPT 1
/** UART0: size of the TX buffer. */
#define UART0_CONF_TX_BUFSIZE      128
/** UART0: custom setup. */
#define UART0_CONF_CUSTOM_INIT       0


/** Use simple RX/TX indicator. */
#define UART1_CONF_RXTX_INDICATOR_SIMPLE 1
/** Doesn't use complex RX/TX indicator. */
#define UART1_CONF_RXTX_INDICATOR_COMPLEX 0
/** UART1: uses interrupt for TX. */
#define UART1_CONF_TX_WITH_INTERRUPT 1
/** UART1: size of the TX buffer. */
#define UART1_CONF_TX_BUFSIZE      128
/** UART1: custom setup. */
#define UART1_CONF_CUSTOM_INIT       0
/** UART1: TX echo on UART0. */
#define UART1_ECHO_TX_ON_UART0       0
/** UART1: RX echo on UART0. */
#define UART1_ECHO_RX_ON_UART0       0
/** UART1: print RX errors flags on UART0 */
#define UART1_PRINT_ERROR_FLAG_ON_UART0 0

/* ----- Serial Line module ----- */
/** Buffer for the serial line reception buffer. */
#define SERIAL_LINE_CONF_BUFSIZE 64
//#define SERIAL_LINE_CONF_IGNORE_CHAR
//#define SERIAL_LINE_CONF_END_CHAR

/* ----- Diagnostic ----- */
/** Diagnostic use a JSON output. */
#define DIAGNOSTIC_CONF_OUTPUT JSON_OUTPUT

/* ----- CC2520 ----- */
/* SPI bus - CC2520 pin configuration. */
#define CC2520_CONF_SYMBOL_LOOP_COUNT 2604      /* 326us msp430X @ 16MHz */

/* P1.6 - Input: FIFOP from CC2520 */
#define CC2520_FIFOP_PORT(type)    P1##type
#define CC2520_FIFOP_PIN           6

/* P1.5 - Input: FIFO from CC2520 */
#define CC2520_FIFO_PORT(type)     P1##type
#define CC2520_FIFO_PIN            5

/* P1.7 - Input: CCA from CC2520 */
#define CC2520_CCA_PORT(type)      P1##type
#define CC2520_CCA_PIN             7

/* P2.0 - Input:  SFD from CC2520 */
#define CC2520_SFD_PORT(type)      P2##type
#define CC2520_SFD_PIN             0

/* P3.0 - Output: SPI Chip Select (CS_N) */
#define CC2520_CSN_PORT(type)      P3##type
#define CC2520_CSN_PIN             0

/* P4.3 - Output: VREG_EN to CC2520 */
#define CC2520_VREG_PORT(type)     P4##type
#define CC2520_VREG_PIN            3

/* P4.4 - Output: RESET_N to CC2520 */
#define CC2520_RESET_PORT(type)    P4##type
#define CC2520_RESET_PIN           4

#define CC2520_IRQ_VECTOR PORT1_VECTOR

/* Pin status.CC2520 */
#define CC2520_FIFOP_IS_1 (!!(CC2520_FIFOP_PORT(IN) & BV(CC2520_FIFOP_PIN)))
#define CC2520_FIFO_IS_1  (!!(CC2520_FIFO_PORT(IN) & BV(CC2520_FIFO_PIN)))
#define CC2520_CCA_IS_1   (!!(CC2520_CCA_PORT(IN) & BV(CC2520_CCA_PIN)))
#define CC2520_SFD_IS_1   (!!(CC2520_SFD_PORT(IN) & BV(CC2520_SFD_PIN)))

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()   (CC2520_RESET_PORT(OUT) |=  BV(CC2520_RESET_PIN))
#define SET_RESET_ACTIVE()     (CC2520_RESET_PORT(OUT) &= ~BV(CC2520_RESET_PIN))

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       (CC2520_VREG_PORT(OUT) |=  BV(CC2520_VREG_PIN))
#define SET_VREG_INACTIVE()     (CC2520_VREG_PORT(OUT) &= ~BV(CC2520_VREG_PIN))

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT() do {                  \
    CC2520_FIFOP_PORT(IES) &= ~BV(CC2520_FIFOP_PIN);  \
    CC2520_CLEAR_FIFOP_INT();                         \
} while(0)

/* FIFOP on external interrupt 0. */
/* FIFOP on external interrupt 0. */
#define CC2520_ENABLE_FIFOP_INT()          do { P1IE |= BV(CC2520_FIFOP_PIN); } while (0)
#define CC2520_DISABLE_FIFOP_INT()         do { P1IE &= ~BV(CC2520_FIFOP_PIN); } while (0)
#define CC2520_CLEAR_FIFOP_INT()           do { P1IFG &= ~BV(CC2520_FIFOP_PIN); } while (0)

/* Enables/disables CC2520 access to the SPI bus (not the bus). (Chip Select) */

/* ENABLE CSn (active low) */
#define CC2520_SPI_ENABLE()     do{ UCB0CTL1 &= ~UCSWRST;  clock_delay(5); P3OUT &= ~BIT0;clock_delay(5);}while(0)
/* DISABLE CSn (active low) */
#define CC2520_SPI_DISABLE()    do{clock_delay(5);UCB0CTL1 |= UCSWRST;clock_delay(1); P3OUT |= BIT0;clock_delay(5);}while(0)
#define CC2520_SPI_IS_ENABLED() ((CC2520_CSN_PORT(OUT) & BV(CC2520_CSN_PIN)) != BV(CC2520_CSN_PIN))

/* include the project config */
/* PROJECT_CONF_H might be defined in the project Makefile */
#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */

#endif /* __CONTIKI_CONF_H__ */

/** @} */

