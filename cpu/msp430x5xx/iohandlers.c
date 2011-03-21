/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \addtogroup iohandlers
 * @{
 */

/**
 * \file
 *         Manager of DI/O Interruption handlers implementation.
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

/* From MSP430-GCC */
#include <stdio.h>
#include <signal.h>

/* From CONTIKI */
#include "sys/energest.h"
#include "dev/watchdog.h"

/* From MSP430x5xx */
#include "contiki-conf.h"
#include "iohandlers.h"

#ifndef IOHANDLERS_CONF_DEBUG
#define DEBUG_IOHANDLERS 0
#else
#define DEBUG_IOHANDLERS (IOHANDLERS_CONF_DEBUG)
#endif


/*---------------------------------------------------------------------------*/

static void
dummyHandler(void)
{
#if DEBUG_IOHANDLERS
  printf("I'm the dummy handler.\n");
#endif /* DEBUG_IOHANDLERS */
}

/*---------------------------------------------------------------------------*/

/** All the IT handlers.
 *
 * Some of them doesn't exist (in the hardware) and are only here to obtain
 * the good index for the correct ones (see the possible values of the
 * PxIV registers).
 */
static ITHandler handlers[17][2] = { { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler },
                                      { dummyHandler, dummyHandler } };

/*---------------------------------------------------------------------------*/

interrupt(PORT1_VECTOR)
irq_p1(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if DEBUG_IOHANDLERS
  printf("Port vector 1\n");
#endif

  handlers[P1IV][0]();

#if DEBUG_IOHANDLERS
  printf("End of port vector 1\n");
#endif

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/*---------------------------------------------------------------------------*/

interrupt(PORT2_VECTOR)
irq_p2(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if DEBUG_IOHANDLERS
  printf("Port vector 2\n");
#endif

  handlers[P2IV][1]();

#if DEBUG_IOHANDLERS
  printf("End of port vector 2\n");
#endif

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/*---------------------------------------------------------------------------*/

/**
 * Set an handler for the corresponding (port,bit).
 * \note "bit" is not the "real" bit. Use *_SET_HANDLER macros.
 *
 * \param handler The new handler
 * \param port    The port
 * \param bit     The bit
 */
void
setHandler(const ITHandler handler, const uint8_t port, const uint8_t bit)
{
#if DEBUG_IOHANDLERS
  printf("I set handlers[%d][%d]\n", bit, port - 1);
#endif
  handlers[bit][port] = handler;
}

/*---------------------------------------------------------------------------*/

/**
 * Reset the handler for the corresponding (port,bit).
 * \note "bit" is not the "real" bit. Use *_SET_HANDLER macros.
 *
 * \param port The port
 * \param bit The bit
 */
void
resetHandler(const uint8_t port, const uint8_t bit)
{
#if DEBUG_IOHANDLERS
  printf("I reset handlers[%d][%d]\n", bit, port - 1);
#endif
  handlers[bit][port] = dummyHandler;
}

/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
