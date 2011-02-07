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
 * \author Anthony Gelibert
 * \date Feb 7, 2011
 * \version 0.0.1
 */

#include <stdio.h>
#include <errno.h>
#include "msp430.h"
#include "uart0.h"
#include "dev/watchdog.h"

/** NOT_YET_DOCUMENTED_PTV */
void
uart0_init(const unsigned int br)
{

}

/** NOT_YET_DOCUMENTED_PTV */
int
uart0_writeb(const unsigned char c)
{
  watchdog_periodic();
  return c;
}

/** NOT_YET_DOCUMENTED_PTV */
uint8_t
uart0_active(void)
{
  return (UCA0STAT & UCBUSY);
}

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
