/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         CC2520 platform-dependent macros.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 23, 2011
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
#include "contiki.h"
#include "contiki-net.h"
#include "net/packetbuf.h"
#include "radio.h"
#include "dev/watchdog.h"

/* From platform */
#include "cc2520.h"
#include "cc2520_const.h"
#include "cc2520_util.h"
#include "cc2520-arch.h"
#include "spi-arch.h"

/* XXX_PTV */
#define TRANSMITTER_ADDR  0x02
#define CHANNEL_SELECTED  20
#define PANID_SELECTED    0xbabe

/** Enable DEBUG */
#define DEBUG 1

#if DEBUG
#include <stdio.h>
/** PRINTF is a real "printf" */
#define PRINTF(...) printf(__VA_ARGS__)
#else
/** PRINTF is deleted. */
#define PRINTF(...)
#endif

/**
 * Time stamp of the last packet.
 */
static volatile uint16_t last_packet_timestamp;

/* XXX_PTV REPLACE THIS */
static void halMcuWaitUs(uint16_t usec) // 5 cycles for calling
{
    // The least we can wait is 3 usec:
    // ~1 usec for call, 1 for first compare and 1 for return
    while(usec > 3)       // 2 cycles for compare
    {                // 2 cycles for jump
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        _NOP();       // 1 cycles for nop
        usec -= 2;        // 1 cycles for optimized decrement
    }
}                         // 4 cycles for returning


/* XXX_PTV REPLACE THIS */
static void halMcuWaitMs(uint16_t msec)
{
    while(msec-- > 0)
    {
        halMcuWaitUs(1000);
    }
}

/* XXX_PTV */
static CC_INLINE void
radioReset(void)
{
  RF_VREG_EN_Px_DIR |= RF_VREG_EN_BIT;
  RF_RSTN_Px_DIR |= RF_RSTN_BIT;

  /* ENABLE */
  RF_VREG_EN_Px_OUT &= ~RF_VREG_EN_BIT;
  RF_VREG_EN_Px_OUT |= RF_VREG_EN_BIT;

  halMcuWaitUs(2100);
  halMcuWaitUs(CC2520_VREG_MAX_STARTUP_TIME);

  /* RESET */
  RF_RSTN_Px_OUT &= ~RF_RSTN_BIT;
  halMcuWaitUs(2100);
  RF_RSTN_Px_OUT |= RF_RSTN_BIT;
  halMcuWaitUs(2100);

  // Enable CS and wait for the chip to be ready
  RF_SPI_CS1N_Px_DIR |= RF_SPI_CS1N_BIT;
  RF_SPI_CS1N_Px_OUT &= ~RF_SPI_CS1N_BIT;

  while (!(SPI_Px_IN & SPI_MISO)) {
    halMcuWaitMs(1);
  }
  RF_SPI_CS1N_Px_OUT |= RF_SPI_CS1N_BIT;
}

static CC_INLINE void
radioInit(void)
{
  //  Recommended TX settings
  /* XXX_PTV Max TX output power */
  CC2520_MEMWR8(CC2520_TXPOWER,  CC2520_TXPOWER_4_DBM);
  // ARAGO VALUE: CC2520_MEMWR8(CC2520_TXCTRL,      0xC1);
  CC2520_MEMWR8(CC2520_TXCTRL,   CC2520_TXCTRL_PLUS_30_C);

  CC2520_MEMWR8(CC2520_CCACTRL0, CC2520_CCA_THRESHOLD_80_DB);

  // Recommended RX settings
  // ARAGO VALUE: CC2520_MEMWR8(CC2520_MDMCTRL0, CC2520_MDMCTRL0_15_4_COMPLIANT);
  CC2520_MEMWR8(CC2520_MDMCTRL0, CC2520_MDMCTRL0_EXTERNAL_FILTER);

  /* CORRELATOR */
  CC2520_MEMWR8(CC2520_MDMCTRL1, CC2520_DEFAULT_CORRELATOR_CORRECTION);

  CC2520_MEMWR8(CC2520_RXCTRL,   CC2520_RXCTRL_NORMAL);

  CC2520_MEMWR8(CC2520_FSCTRL,   CC2520_FSCTRL_NORMAL);

  // ARAGO VALUE: CC2520_MEMWR8(CC2520_FSCAL1, 0x03);
  CC2520_MEMWR8(CC2520_FSCAL1,   CC2520_VCO_DEFAULT);

  /* TODO_PTV: A modifier pour mode low-power.
   * QUAND ON FERA LE MODE LOW POWER, IL FAUDRA AJOUTER : CC2520_MEMWR8(CC2520_AGCCTRL2, CC2520_AGC2_LOW_POWER);
   */
  CC2520_MEMWR8(CC2520_AGCCTRL1, CC2520_AGC1_DEFAULT);

  // TI MAGIC VALUES
  CC2520_MEMWR8(CC2520_ADCTEST0,    0x10);
  CC2520_MEMWR8(CC2520_ADCTEST1,    0x0E);
  CC2520_MEMWR8(CC2520_ADCTEST2,    0x03);

  /* XXX_PTV */
  CC2520_REGWR8(CC2520_FREQCTRL, CHANNEL_SELECTED);

  /* XXX_PTV */
  CC2520_MEMWR16(CC2520_RAM_SHORTADDR, TRANSMITTER_ADDR);

  /* XXX_PTV */
  CC2520_MEMWR16(CC2520_RAM_PANID, PANID_SELECTED);

  /* XXX_PTV Probleme Coordinator */
  /* Frame Filter. */
  // ARAGO VALUE: CC2520_MEMWR8(CC2520_FRMFILT0, CC2520_FRAME_FILTER_DISABLED);
  CC2520_MEMWR8(CC2520_FRMFILT0, CC2520_FRAME_FILTER_DEFAULT);

  //  Set auto CRC on frame
  // ARAGO_VALUE: CC2520_MEMWR8(CC2520_FRMCTRL0,    0x60);
  CC2520_MEMWR8(CC2520_FRMCTRL0, CC2520_FRAME_CONTROL_DEFAULT);
}

int cc2520_init(void)
{
  radioReset();
  watchdog_stop();
  PRINTF("RF_INIT()\n");
  radioInit();
  return 0;
}

/**
 * Get the current CC2520 Chip Version.
 *
 * @return Chip Version.
 */
uint16_t cc2520_version(void)
{
  return CC2520_REGRD16(CC2520_VERSION);
}

/**
 * Get the current CC2520 Chip ID.
 *
 * @return Chip ID.
 */
uint16_t cc2520_chipid(void)
{
  return CC2520_REGRD16(CC2520_CHIPID);
}

/**
 * Prepare the radio with a packet to be sent.
 */
static int cc2520_prepare(const void *payload, unsigned short payload_len)
{
  return 0;
}

/**
 * Send the packet that has previously been prepared.
 */
static int cc2520_transmit(unsigned short transmit_len)
{
  return 0;
}

/**
 * Prepare & transmit a packet.
 */
static int cc2520_send(const void *payload, unsigned short payload_len)
{
  return 0;
}

/**
 * Read a received packet into a buffer.
 */
static int cc2520_read(void *buf, unsigned short buf_len)
{
  return 0;
}

/**
 * Perform a Clear-Channel Assessment (CCA) to find out if there is
 * a packet in the air or not.
 */
static int cc2520_channel_clear(void)
{
  return 0;
}

/**
 * Check if the radio driver is currently receiving a packet.
 */
static int cc2520_receiving_packet(void)
{
  return 0;
}

/**
 * Check if the radio driver has just received a packet.
 */
static int cc2520_pending_packet(void)
{
  return 0;
}

/**
 * Turn the radio on.
 */
int cc2520_on(void)
{
  return 0;
}

/**
 * Turn the radio off.
 */
int cc2520_off(void)
{
  return 0;
}

/**
 * CC2520 Radio Driver.
 */
const struct radio_driver cc2520_driver =
{
 cc2520_init,
 cc2520_prepare,
 cc2520_transmit,
 cc2520_send,
 cc2520_read,
 cc2520_channel_clear,
 cc2520_receiving_packet,
 cc2520_pending_packet,
 cc2520_on,
 cc2520_off,
};

/*---------------------------------------------------------------------------*/
PROCESS(cc2520_process, "CC2520 driver");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cc2520_process, ev, data)
{
  int len;
  PROCESS_BEGIN();

  PRINTF("cc2520_process: started\n");
  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
    PRINTF("cc2520_process: calling receiver callback\n");
    packetbuf_clear();
    packetbuf_set_attr(PACKETBUF_ATTR_TIMESTAMP, last_packet_timestamp);
    len = cc2520_read(packetbuf_dataptr(), PACKETBUF_SIZE);
    packetbuf_set_datalen(len);
    NETSTACK_RDC.input();
  }

  PROCESS_END();
}

/** @} */
