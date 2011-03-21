/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         SPI platform-dependent implementation.
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

/* From CONTIKI */
#include "spi.h"

/* From platform */
#include "contiki-conf.h"
#include "spi-arch.h"

unsigned char spi_busy = 0;

void
spi_init(void)
{
  /* === Put state machine in reset === */
  UCB0CTL1 |= UCSWRST;

  UCB0CTL1 = UCSSEL__SMCLK;
  UCB0CTL0 |=  UCCKPH | UCSYNC | UCMSB | UCMST; // 3-pin, 8-bit SPI master, rising edge capture

  /* SMCLK / (UCxxBR0 + UCxxBR1 x 256)  */
  UCB0BRW = 0x08;

  // Set MOSI and SCLK as OUT and MISO as IN ports
  SPI_Px_SEL |= (SPI_MOSI | SPI_MISO | SPI_CLK); // Port3 = SPI peripheral
  SPI_Px_DIR |= (SPI_MOSI | SPI_CLK);            // MOSI and SCLK as Output
  SPI_Px_DIR &= ~SPI_MISO;                       // Don't forget to configure MISO as Input
  UCB0IE |= UCTXIE | UCRXIE;                     // Enable interrupt for RX and TX SPI buffer

  /* === Initialize USCI state machine === */
  UCB0CTL1 &= ~UCSWRST;
}

/** @} */
