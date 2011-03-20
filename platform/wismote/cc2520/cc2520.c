#include "contiki.h"
#include "radio.h"
#include "cc2520_const-arch.h"
#include "spi-arch.h"

#define DEBUG 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...) do {} while (0)
#endif

int cc2520_init(void);
static int cc2520_prepare(const void *payload, unsigned short payload_len);
static int cc2520_transmit(unsigned short transmit_len);
static int cc2520_send(const void *payload, unsigned short payload_len);
static int cc2520_read(void *buf, unsigned short buf_len);
static int cc2520_channel_clear(void);
static int cc2520_receiving_packet(void);
static int cc2520_pending_packet(void);
int cc2520_on(void);
int cc2520_off(void);

const struct radio_driver cc2520_driver =
{
 cc2520_init,
 cc2520_prepare,
 cc2520_transmit,
 cc2520_send,
 cc2520_read,
 cc2520_cca,
 cc2520_receiving_packet,
 cc2520_pending_packet,
 cc2520_on,
 cc2520_off,
};

static void halMcuWaitUs(uint16 usec) // 5 cycles for calling
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


/* XXX_PTV */
static void
RF_RESET(void)
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
  /* XXX_PTV Decoupage de SPI_CC2520 */
  while (!SPI_Px_IN & SPI_MISO) {
    halMcuWaitMs(1);
  }
  RF_SPI_CS1N_Px_OUT |= RF_SPI_CS1N_BIT;
}

static void RF_INITIALIZATION()
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
  CC2520_REGWR8(CC2520_FREQCTRL, CHANNEL_SELECTED);           //  Define channel (between 11 and 25(?to check))

  /* XXX_PTV */
  CC2520_MEMWR16(CC2520_RAM_SHORTADDR, TRANSMITTER_ADDR); //  Define short address of the node

  /* XXX_PTV */
  CC2520_MEMWR16(CC2520_RAM_PANID, PANID_SELECTED);     //  Define the pan ID of the network

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
  RF_RESET();
  RF_INITIALIZATION();
}

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
    len = cc2420_read(packetbuf_dataptr(), PACKETBUF_SIZE);

    packetbuf_set_datalen(len);

    NETSTACK_RDC.input();
  }

  PROCESS_END();
}
