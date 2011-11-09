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
 *         UART1 implementation.
 * \author
 *         Fabien Rey <fabien-rey@wanadoo.fr>
 * \date
 *         April 15, 2011
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
#include "uart1.h"

#if UART1_CONF_RXTX_INDICATOR_SIMPLE && UART1_CONF_RXTX_INDICATOR_COMPLEX
    #error "Enable only \"simple\" OR \"complex\" RXTX_INDICATOR for UART1"
#endif

#if !UART1_CONF_RXTX_INDICATOR_SIMPLE && !UART1_CONF_RXTX_INDICATOR_COMPLEX
    #warning "No led toggling enabled for UART1 RX/TX operation"
#endif

#ifdef UART1_CONF_CUSTOM_INIT
#define UART1_CUSTOM_INIT (UART1_CONF_CUSTOM_INIT)
#else
#define UART1_CUSTOM_INIT 0
#endif

#ifdef UART1_CONF_TX_WITH_INTERRUPT
/** Size of the TX buffer size. */
#define UART1_TX_WITH_INTERRUPT (UART1_CONF_TX_WITH_INTERRUPT)
#else
/** By default, UART1 doesn't use interrupt. */
#define UART1_TX_WITH_INTERRUPT 0
#endif /* UART1_CONF_TX_WITH_INTERRUPT */

#if UART1_TX_WITH_INTERRUPT
#ifdef UART1_CONF_TX_BUFSIZE
/** Size of the TX buffer size. */
#define UART1_TXBUFSIZE (UART1_CONF_TX_BUFSIZE)
#else
/** By default, TX buffer size is 64 bytes. */
#define UART1_TXBUFSIZE 64
#endif
/** Indicate a transmitting. */
static volatile uint8_t transmitting;
/** TX buffer. */
static struct ringbuf txbuf;
/** Data of the TX buffer */
static uint8_t txbuf_data[UART1_TXBUFSIZE];
#endif /* UART1_TX_WITH_INTERRUPT */

/** Handler for the char reception.
 *
 * \param c Received char.
 * \return ...
 */
static int
(* uart1_input_handler)(const uint8_t c);

/** UART1 platform-dependent code. */
extern void
uart1_arch_init();

#if UART1_CUSTOM_INIT
/** UART1 application-dependent code. */
extern void
uart1_custom_init();
#endif

/*---------------------------------------------------------------------------*/

/**
 * \brief Initialize the UART1.
 *
 * \param br The UCA1BRW value.
 * \param brs The UCBRSx value.
 * \param brf The UCBRFx value.
 */
void
uart1_init(const uint16_t br, const uint8_t brs, const uint8_t brf)
{
  /* See MSP430x5xx/6xx Family User's Guide p. 577 */

  /* -- Put state machine in reset -- */
  UCA0CTL1 |= UCSWRST;

#if UART1_CUSTOM_INIT
  uart1_custom_init();
#else
  /* Choose SMCLK */
  UCA0CTL1 |= UCSSEL__SMCLK;
  /* Set baudrate */
  UCA0BRW = br;
  /* Modulation */
  UCA0MCTL |= brs | brf;
#endif
  /* Clear pending flags. */
  UCA0IFG &= ~UCRXIFG;
  UCA0IFG &= ~UCTXIFG;

  uart1_arch_init();

  /* -- Initialize USCI state machine -- */
  UCA0CTL1 &= ~UCSWRST;

  /* USCI Receive Interrupt Enable */
  UCA0IE |= UCRXIE;

#if UART1_TX_WITH_INTERRUPT
  /* We don't transmit */
  transmitting = 0;
  /* USCI Transmit Interrupt Enable */
  ringbuf_init(&txbuf, txbuf_data, sizeof(txbuf_data));
  UCA0IE |= UCTXIE;
#endif /* UART1_TX_WITH_INTERRUPT */
}

/*---------------------------------------------------------------------------*/

/** Write a byte on the UART1.
 *
 * \param c The byte to send.
 * \return c
 */
int
uart1_writeb(const uint8_t c)
{
  watchdog_periodic();
#if UART1_ECHO_TX_ON_UART0
  uart0_writeb(c);
#endif
#if UART1_TX_WITH_INTERRUPT
  /* Put the outgoing byte on the transmission buffer. If the buffer
   is full, we just keep on trying to put the byte into the buffer
   until it is possible to put it there. */
  while (ringbuf_put(&txbuf, c) == 0) {
  }

  /* If there is no transmission going, we need to start it by putting
   the first byte into the UART. */
  if (transmitting == 0) {
    transmitting = 1;
    UCA0TXBUF = ringbuf_get(&txbuf);
  }
#else
  /* Loop until the transmission buffer is available. */
  while((UCA0STAT & UCBUSY)) {
  }
  /* Transmit the data. */
  UCA0TXBUF = c;
#endif /* UART1_TX_WITH_INTERRUPT */

  return c;
}

/*---------------------------------------------------------------------------*/

/** Test the activity of UART1
 *
 * \return Result of the test
 */
uint8_t
uart1_active(void)
{
#if UART1_CONF_TX_WITH_INTERRUPT
  return (UCA0STAT & UCBUSY) | transmitting;
#else
  return (UCA0STAT & UCBUSY);
#endif
}

/*---------------------------------------------------------------------------*/

/**
 * Set the UART1 RX handler.
 *
 * @param input The handler
 */
void
uart1_set_input(int
(* input)(const uint8_t c))
{
  uart1_input_handler = input;
}

/*---------------------------------------------------------------------------*/

#if !WITH_UIP && STDIO_USE_UART1
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
  return uart1_writeb(c);
}
#endif

/*---------------------------------------------------------------------------*/

/**
 * UART A0 interrupt vector (RX/TX).
 *
 * @param USCI_A0_VECTOR Interrupt vector USCI A1
 */
interrupt(USCI_A0_VECTOR)
uart1_interrupt(void)
{
  uint8_t c;

  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if UART1_CONF_RXTX_INDICATOR_SIMPLE
  /* Indicate a rx/tx operation */
  leds_toggle(LEDS_RED);
#endif
  /* Test a RX */
  if (UCA0IFG & UCRXIFG) {
    /* Check for an error */
    if (UCA0STAT & UCRXERR) {
#if UART1_CONF_RXTX_INDICATOR_COMPLEX
      /* Indicate a rx/tx operation */
      leds_toggle(LEDS_RED);
#endif
#if UART1_PRINT_ERROR_FLAG_ON_UART0
      if (UCA0STAT & UCFE)
      {
          uart0_writeb('F');
      }
      if (UCA0STAT & UCOE)
      {
          uart0_writeb('O');
      }
      if (UCA0STAT & UCPE)
      {
          uart0_writeb('P');
      }
#endif
      /* Clear error flags by forcing a dummy read. */
      c = UCA0RXBUF;
    } else {
#if UART1_CONF_RXTX_INDICATOR_COMPLEX
      /* Indicate a rx/tx operation */
      leds_toggle(LEDS_GREEN);
#endif
      /* Get the good value */
      c = UCA0RXBUF;
#if UART1_ECHO_RX_ON_UART0
      uart0_writeb(c);
#endif
      /* If an input handler is set, use it */
      if (uart1_input_handler != NULL) {
        if (uart1_input_handler(c)) {
          LPM4_EXIT;
        }
      }
    }
  }
/* If we use TX interrupt, also check it */
#if UART1_TX_WITH_INTERRUPT
  else {
    /* Checking */
    if (UCA0IFG & UCTXIFG) {
      /* Last byte */
      if (ringbuf_elements(&txbuf) == 0) {
        transmitting = 0;
      } else {
#if UART1_CONF_RXTX_INDICATOR_COMPLEX
        leds_toggle(LEDS_BLUE);
#endif
        UCA0TXBUF = ringbuf_get(&txbuf);
      }
      /* In a stand-alone app won't work without this. Is the UG misleading? */
      UCA0IFG &= ~UCTXIFG;
    }
  }
#endif /* UART1_TX_WITH_INTERRUPT */

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
