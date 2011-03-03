/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \defgroup iohandlers Digital I/O
 *
 * This is the module multiplexing/demultiplexing the two port's interrupt
 * vectors.<br/>
 * It uses dummy function for the inactive ports and can assign user-defined
 * handlers.
 *
 * @{
 */

/**
 * \file
 *         Manager of DI/O Interruption handlers routines.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 03, 2011
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

#ifndef __IOHANDLERS_H__
#define __IOHANDLERS_H__

#include "sys/cc.h"
#include "iohandlers.h"

#define HWCONF_PIN(name, port, bit)                                            \
static CC_INLINE void name##_SELECT()      {P##port##SEL &= ~BIT##bit;}        \
static CC_INLINE void name##_SELECT_IO()   {P##port##SEL &= ~BIT##bit;}        \
static CC_INLINE void name##_SELECT_PM()   {P##port##SEL |=  BIT##bit;}        \
static CC_INLINE void name##_SET()         {P##port##OUT |=  BIT##bit;}        \
static CC_INLINE void name##_CLEAR()       {P##port##OUT &= ~BIT##bit;}        \
static CC_INLINE void name##_RESISTOR_ENABLE()   {P##port##REN |=  BIT##bit;}  \
static CC_INLINE void name##_RESISTOR_DISABLE()  {P##port##REN &= ~BIT##bit;}  \
static CC_INLINE void name##_RESISTOR_PULL_UP()  {P##port##OUT |= BIT##bit;}   \
static CC_INLINE void name##_RESISTOR_PULL_DOWN() {P##port##OUT &= ~BIT##bit;}  \
static CC_INLINE void name##_RESISTOR_PULL_SWAP() {P##port##OUT ^= BIT##bit;}   \
static CC_INLINE int  name##_READ()        {return (P##port##IN & BIT##bit);}  \
static CC_INLINE void name##_MAKE_OUTPUT() {P##port##DIR |= BIT##bit;}         \
static CC_INLINE void name##_MAKE_INPUT()  {P##port##DIR &= ~BIT##bit;}

#define HWCONF_IRQ(name, port, bit, handler)                                   \
static CC_INLINE void name##_ENABLE_IRQ()       {P##port##IE |= BIT##bit;}     \
static CC_INLINE void name##_DISABLE_IRQ()      {P##port##IE &= ~BIT##bit;}    \
static CC_INLINE int  name##_IRQ_ENABLED()    {return P##port##IE & BIT##bit;} \
static CC_INLINE void name##_IRQ_EDGE_SELECTD() {P##port##IES |= BIT##bit;}    \
static CC_INLINE void name##_IRQ_EDGE_SELECTU() {P##port##IES &= ~BIT##bit;}   \
static CC_INLINE void name##_IRQ_EDGE_SWAP()    {P##port##IES ^= BIT##bit;}    \
static CC_INLINE int  name##_CHECK_IRQ()      {return P##port##IFG & BIT##bit;}\
static CC_INLINE int  name##_IRQ_PORT()       {return port;}                   \
static CC_INLINE void name##_SET_HANDLER()      {setHandler(handler,port-1,P##port##IV_P##port##IFG##bit);}\
static CC_INLINE void name##_RESET_HANDLER()    {resetHandler(port-1,P##port##IV_P##port##IFG##bit);}

/** IT interruption handler. */
typedef void (*ITHandler)(void);

/**
 * Set an handler for the corresponding (port,bit).
 * \note "bit" is not the "real" bit. Use *_SET_HANDLER macros.
 *
 * \param handler The new handler
 * \param port    The port
 * \param bit     The bit
 */
void setHandler(const ITHandler handler, const uint8_t port, const uint8_t bit);

/**
 * Reset the handler for the corresponding (port,bit).
 * \note "bit" is not the "real" bit. Use *_SET_HANDLER macros.
 *
 * \param port The port
 * \param bit The bit
 */
void resetHandler(const uint8_t port, const uint8_t bit);
#endif /* __IOHANDLERS_H__ */

/** @} */
/** @} */
