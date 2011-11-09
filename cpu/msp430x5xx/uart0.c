/**
 * \addtogroup msp430x5xx
 * @{
 */

/**
 * \addtogroup uart
 * @{
 */

/**
 * \file
 *         UART0 implementation.
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

/* From MSP430-GCC */
#include <stdio.h>
#include <errno.h>
#include <legacymsp430.h>

/* From CONTIKI */
#include "contiki.h"
#include "dev/watchdog.h"
#include "dev/leds.h"
#include "lib/ringbuf.h"

/* From MSP430x5xx */
#include "msp430.h"
#include "uart0.h"

#if UART0_CONF_RXTX_INDICATOR_SIMPLE && UART0_CONF_RXTX_INDICATOR_COMPLEX
    #error "Enable only \"simple\" OR \"complex\" RXTX_INDICATOR for UART0"
#endif

#if !UART0_CONF_RXTX_INDICATOR_SIMPLE && !UART0_CONF_RXTX_INDICATOR_COMPLEX
    #warning "No led toggling enabled for UART0 RX/TX operation"
#endif

#ifdef UART0_CONF_CUSTOM_INIT
#define UART0_CUSTOM_INIT (UART0_CONF_CUSTOM_INIT)
#else
#define UART0_CUSTOM_INIT 0
#endif

#ifdef UART0_CONF_TX_WITH_INTERRUPT
/** Size of the TX buffer size. */
#define TX_WITH_INTERRUPT (UART0_CONF_TX_WITH_INTERRUPT)
#else
/** By default, UART0 doesn't use interrupt. */
#define TX_WITH_INTERRUPT 0
#endif /* UART0_CONF_TX_WITH_INTERRUPT */

#if TX_WITH_INTERRUPT
#ifdef UART0_CONF_TX_BUFSIZE
/** Size of the TX buffer size. */
#define TXBUFSIZE (UART0_CONF_TX_BUFSIZE)
#else
/** By default, TX buffer size is 64 bytes. */
#define TXBUFSIZE 64
#endif
/** Indicate a transmitting. */
static volatile uint8_t transmitting;
/** TX buffer. */
static struct ringbuf txbuf;
/** Data of the TX buffer */
static uint8_t txbuf_data[TXBUFSIZE];
#endif /* TX_WITH_INTERRUPT */

/** Handler for the char reception.
 *
 * \param c Received char.
 * \return ...
 */
static int
(* uart0_input_handler)(const uint8_t c);

/** UART0 platform-dependent code. */
extern void
uart0_arch_init();

#if UART0_CUSTOM_INIT
/** UART0 application-dependent code. */
extern void
uart0_custom_init();
#endif

/*---------------------------------------------------------------------------*/

/**
 * \brief Initialize the UART0.
 *
 * \param br The UCA1BRW value.
 * \param brs The UCBRSx value.
 * \param brf The UCBRFx value.
 */
void
uart0_init(const uint16_t br, const uint8_t brs, const uint8_t brf)
{
  /* See MSP430x5xx/6xx Family User's Guide p. 577 */

  /* -- Put state machine in reset -- */
  UCA1CTL1 |= UCSWRST;

#if UART0_CUSTOM_INIT
  uart0_custom_init();
#else
  /* Choose SMCLK */
  UCA1CTL1 |= UCSSEL__SMCLK;
  /* Set baudrate */
  UCA1BRW = br;
  /* Modulation */
  UCA1MCTL |= brs | brf;
#endif

  /* Clear pending flags. */
  UCA1IFG &= ~UCRXIFG;
  UCA1IFG &= ~UCTXIFG;

  uart0_arch_init();

  /* -- Initialize USCI state machine -- */
  UCA1CTL1 &= ~UCSWRST;

  /* USCI Receive Interrupt Enable */
  UCA1IE |= UCRXIE;

#if TX_WITH_INTERRUPT
  /* We don't transmit */
  transmitting = 0;
  /* USCI Transmit Interrupt Enable */
  ringbuf_init(&txbuf, txbuf_data, sizeof(txbuf_data));
  UCA1IE |= UCTXIE;
#endif /* TX_WITH_INTERRUPT */
}

/*---------------------------------------------------------------------------*/

/** Write a byte on the UART0.
 *
 * \param c The byte to send.
 * \return c
 */
int
uart0_writeb(const uint8_t c)
{
  watchdog_periodic();

#if TX_WITH_INTERRUPT
  /* Put the outgoing byte on the transmission buffer. If the buffer
   is full, we just keep on trying to put the byte into the buffer
   until it is possible to put it there. */
  while (ringbuf_put(&txbuf, c) == 0) {
  }

  /* If there is no transmission going, we need to start it by putting
   the first byte into the UART. */
  if (transmitting == 0) {
    transmitting = 1;
    UCA1TXBUF = ringbuf_get(&txbuf);
  }
#else
  /* Loop until the transmission buffer is available. */
  while((UCA1STAT & UCBUSY)) {
  }
  /* Transmit the data. */
  UCA1TXBUF = c;
#endif /* TX_WITH_INTERRUPT */

  return c;
}

/*---------------------------------------------------------------------------*/

/** Test the activity of UART0
 *
 * \return Result of the test
 */
uint8_t
uart0_active(void)
{
#if UART0_CONF_TX_WITH_INTERRUPT
  return (UCA1STAT & UCBUSY) | transmitting;
#else
  return (UCA1STAT & UCBUSY);
#endif
}

/*---------------------------------------------------------------------------*/

/**
 * Set the UART0 RX handler.
 *
 * @param input The handler
 */
void
uart0_set_input(int
(* input)(const uint8_t c))
{
  uart0_input_handler = input;
}

/*---------------------------------------------------------------------------*/

#if !WITH_UIP && !STDIO_USE_UART1
/**
 * \brief Writes character to the current position in the standard output.
 *
 * Writes character to the current position in the standard output (stdout)
 * and advances the internal file position indicator to the next position.
 * It is equivalent to putc(character,stdout).
 *
 * \param c Character to be written. The character is passed as its int promotion.
 *
 * \retval c No errors.
 * \retval EOF Error occurs and the error indicator is set.
 */
int
putchar(int c)
{
  if (c < 0 || c > 255) {
    errno = EINVAL;
    return EOF;
  }
  return uart0_writeb(c);
}
#endif

/*---------------------------------------------------------------------------*/

/**
 * UART A1 interrupt vector (RX/TX).
 *
 * @param USCI_A1_VECTOR Interrupt vector USCI A1
 */
interrupt(USCI_A1_VECTOR)
uart0_interrupt(void)
{
  uint8_t c;

  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if UART0_CONF_RXTX_INDICATOR_SIMPLE
  /* Indicate a rx/tx operation */
  leds_toggle(LEDS_RED);
#endif
  /* Test a RX */
  if (UCA1IFG & UCRXIFG) {
    /* Check for an error */
    if (UCA1STAT & UCRXERR) {
#if UART0_CONF_RXTX_INDICATOR_COMPLEX
      /* Indicate a rx/tx operation */
      leds_toggle(LEDS_RED);
#endif
      /* Clear error flags by forcing a dummy read. */
      c = UCA1RXBUF;
    } else {
#if UART0_CONF_RXTX_INDICATOR_COMPLEX
      /* Indicate a rx/tx operation */
      leds_toggle(LEDS_GREEN);
#endif
      /* Get the good value */
      c = UCA1RXBUF;
      /* If an input handler is set, use it */
      if (uart0_input_handler != NULL) {
        if (uart0_input_handler(c)) {
          LPM4_EXIT;
        }
      }
    }
  }
/* If we use TX interrupt, also check it */
#if TX_WITH_INTERRUPT
  else {
    /* Checking */
    if (UCA1IFG & UCTXIFG) {
      /* Last byte */
      if (ringbuf_elements(&txbuf) == 0) {
        transmitting = 0;
      } else {
#if UART0_CONF_RXTX_INDICATOR_COMPLEX
        leds_toggle(LEDS_BLUE);
#endif
        UCA1TXBUF = ringbuf_get(&txbuf);
      }
      /* In a stand-alone app won't work without this. Is the UG misleading? */
      UCA1IFG &= ~UCTXIFG;
    }
  }
#endif /* TX_WITH_INTERRUPT */

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
