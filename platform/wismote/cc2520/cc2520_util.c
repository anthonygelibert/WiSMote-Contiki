#include "cc2520_util.h"
#include "cc2520_const.h"

// SPI register definitions
#define CC2520_SPI_TX_REG               (UCB0TXBUF)
#define CC2520_SPI_RX_REG               (UCB0RXBUF)
#define CC2520_SPI_RX_IS_READY()        (UCB0IFG & UCRXIFG)
#define CC2520_SPI_RX_NOT_READY()       (UCB0IFG &= ~UCRXIFG)

// SPI access macros
#define CC2520_SPI_TX(x)                st( CC2520_SPI_RX_NOT_READY(); CC2520_SPI_TX_REG = x; )
#define CC2520_SPI_RX()                 (CC2520_SPI_RX_REG)
#define CC2520_SPI_WAIT_RXRDY()         st( while (!(CC2520_SPI_RX_IS_READY())); )

// SPI interface control
#define CC2520_SPI_BEGIN()              P3OUT &= ~BIT0;\
                                        _NOP(); \
                                        _NOP(); \
                                        _NOP();

#define CC2520_SPI_END()                _NOP(); \
                                        _NOP(); \
                                        _NOP(); \
                                        P3OUT |= BIT0; \
                                        _NOP(); \
                                        _NOP(); \
                                        _NOP();


uint8 CC2520_MEMWR(uint16 addr, uint16 count, uint8  *pData)
{
    uint8 s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_INS_WR_ARRAY(count, pData);
    CC2520_SPI_END();
    return s;
}

uint8 CC2520_MEMWR8(uint16 addr, uint8 value)
{
    uint8 s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(value);
    CC2520_SPI_END();
    return s;
}

uint8 CC2520_MEMWR16(uint16 addr, uint16 value)
{
    uint8 s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(value));
    CC2520_SPI_TXRX(HI_UINT16(value));
    CC2520_SPI_END();
    return s;
}

uint8 CC2520_MEMWR24(uint16 addr, uint32 value)
{
    uint8 s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(HI_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(LO_UINT16(HI_UINT32(value)));
    CC2520_SPI_END();
    return s;
}

