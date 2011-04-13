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

#include "contiki-conf.h"
#include "cc2520_util.h"
#include "cc2520_const.h"
#include "spi-arch.h"

// SPI register definitions
#define CC2520_SPI_RX_IS_READY()        (UCB0IFG & UCRXIFG)
#define CC2520_SPI_RX_NOT_READY()       (UCB0IFG &= ~UCRXIFG)

// SPI access macros
#define CC2520_SPI_TX(x)                CC2520_SPI_RX_NOT_READY(); \
                                        SPI_TXBUF = x;
#define CC2520_SPI_RX()                 SPI_RXBUF
#define CC2520_SPI_WAIT_RXRDY()         while (!(CC2520_SPI_RX_IS_READY()));

/** Start of an SPI communication. */
#define CC2520_SPI_BEGIN()              SPI_Px_OUT &= ~SPI_CS1N; \
                                        _NOP(); \
                                        _NOP(); \
                                        _NOP();

/** End of an SPI communication. */
#define CC2520_SPI_END()                _NOP(); \
                                        _NOP(); \
                                        _NOP(); \
                                        SPI_Px_OUT |= SPI_CS1N; \
                                        _NOP(); \
                                        _NOP(); \
                                        _NOP();

static uint8_t CC2520_SPI_TXRX(const uint8_t x)
{
    CC2520_SPI_TX(x);
    CC2520_SPI_WAIT_RXRDY();
    return CC2520_SPI_RX();
}

/*******************************************************************************
* @fn      CC2520_MEMWR
*
* @brief   Write memory
*
* @param   uint16_t addr
*          uint16_t count
*          uint8_t  *pData
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_MEMWR(uint16_t addr, uint16_t count, uint8_t  *pData)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_INS_WR_ARRAY(count, pData);
    CC2520_SPI_END();
    return s;
}

/*******************************************************************************
* @fn      CC2520_MEMWR8
*
* @brief   Write memory 8 bits
*
* @param   uint16_t addr
*          uint8_t value
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_MEMWR8(uint16_t addr, uint8_t value)
{
    uint8_t s;

    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(value);
    CC2520_SPI_END();
    return s;
}

/*******************************************************************************
* @fn      CC2520_MEMWR16
*
* @brief   Write memory 16 bits
*
* @param   uint16_t addr
*          uint16_t value
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_MEMWR16(uint16_t addr, uint16_t value)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(value));
    CC2520_SPI_TXRX(HI_UINT16(value));
    CC2520_SPI_END();
    return s;
}

/*******************************************************************************
* @fn      CC2520_MEMWR24
*
* @brief   Write memory 24 bits
*
* @param   uint16_t addr
*          uint32_t value
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_MEMWR24(uint16_t addr, uint32_t value)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMWR | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(HI_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(LO_UINT16(HI_UINT32(value)));
    CC2520_SPI_END();
    return s;
}

/*******************************************************************************
* @fn      CC2520_MEMRD
*
* @brief   Read memory
*
* @param   uint16_t addr
*          uint16_t count
*          uint8_t  *pData
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_MEMRD(uint16_t addr, uint16_t count, uint8_t  *pData)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_MEMRD | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    CC2520_INS_RD_ARRAY(count, pData);
    CC2520_SPI_END();
    return s;
}


/******************************************************************************
* @fn      CC2520_MEMRD8
*
* @brief   Read memory 8 bits
*
* @param   uint16_t addr
*
* @return  uint8_t - result
*/
uint8_t CC2520_MEMRD8(uint16_t addr)
{
    uint8_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_MEMRD | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    value = CC2520_SPI_TXRX(0x00);
    CC2520_SPI_END();
    return value;
}


/*******************************************************************************
* @fn      CC2520_MEMRD16
*
* @brief   Read memory 16 bits
*
* @param   uint16_t addr
*
* @return  uint16_t - result
*/
uint16_t CC2520_MEMRD16(uint16_t addr)
{
    uint16_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_MEMRD | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    value = (uint32_t)(CC2520_SPI_TXRX(0x00));
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 8;
    CC2520_SPI_END();
    return value;
}


/*******************************************************************************
* @fn      CC2520_MEMRD24
*
* @brief   Read memory 24 bits
*
* @param   uint16_t addr
*
* @return  uint32_t - result
*/
uint32_t CC2520_MEMRD24(uint16_t addr)
{
    uint32_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_MEMRD | HI_UINT16(addr));
    CC2520_SPI_TXRX(LO_UINT16(addr));
    value =  (uint32_t)(CC2520_SPI_TXRX(0x00));
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 8;
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 16;
    CC2520_SPI_END();
    return value;
}

/***********************************************************************************
* @fn      CC2520_REGRD
*
* @brief   Register read. Can only be started from addresses below 0x40
*
* @param  uint8_t addr - address
*         uint8_t count - number of bytes
*         uint8_t  *pValues - buffer to store result
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_REGRD(uint8_t addr, uint8_t count, uint8_t  *pValues)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_REGRD | addr);
    CC2520_INS_RD_ARRAY(count, pValues);
    CC2520_SPI_END();
    return s;
}


/***********************************************************************************
* @fn      CC2520_REGRD8
*
* @brief   Read one register byte
*
* @param  uint8_t addr - address
*
* @return  uint8_t - result
*/
uint8_t CC2520_REGRD8(uint8_t addr)
{
    uint8_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGRD | addr);
    value = CC2520_SPI_TXRX(0x00);
    CC2520_SPI_END();
    return value;
}


/***********************************************************************************
* @fn      CC2520_REGRD16
*
* @brief   Read two register bytes
*
* @param  uint8_t addr - address
*
* @return  uint16_t - result
*/
uint16_t CC2520_REGRD16(uint8_t addr)
{
    uint16_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGRD | addr);
    value = CC2520_SPI_TXRX(0x00);
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 8;
    CC2520_SPI_END();
    return value;
}


/***********************************************************************************
* @fn      CC2520_REGRD24
*
* @brief   Read three register bytes
*
* @param  uint8_t addr - address
*
* @return  uint32_t - result
*/
uint32_t CC2520_REGRD24(uint8_t addr)
{
    uint32_t value;
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGRD | addr);
    value = CC2520_SPI_TXRX(0x00);
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 8;
    value |= (uint32_t)(CC2520_SPI_TXRX(0x00)) << 16;
    CC2520_SPI_END();
    return value;
}

/***********************************************************************************
* @fn      CC2520_INS_WR_ARRAY
*
* @brief   Write array to CC2520
*
* @param   uint16_t count -
*          uint8_t  *pData -
*
* @return  none
*/
void CC2520_INS_WR_ARRAY(uint16_t count, uint8_t  *pData)
{
    while (count--) {
        CC2520_SPI_TX(*pData);
        pData++;
        CC2520_SPI_WAIT_RXRDY();
    }
}

/***********************************************************************************
* @fn      CC2520_INS_RD_ARRAY
*
* @brief   Read array from CC2520
*
* @param   uint16_t count -
*          uint8_t  *pData -
*
* @return  none
*/
void CC2520_INS_RD_ARRAY(uint16_t count, uint8_t  *pData)
{
    while (count--) {
        CC2520_SPI_TX(0x00);
        CC2520_SPI_WAIT_RXRDY();
        *pData = CC2520_SPI_RX();
        pData++;
    }
}

/***********************************************************************************
* @fn      CC2520_REGWR
*
* @brief   Register write. Can only be started from addresses below 0x40
*
* @param  uint8_t addr - address
*         uint8_t count - number of bytes
*         uint8_t  *pValues - data buffer
*
* @return  uint8_t - status byte
*/
uint8_t CC2520_REGWR(uint8_t addr, uint8_t count, uint8_t  *pValues)
{
    uint8_t s;
    CC2520_SPI_BEGIN();
    s = CC2520_SPI_TXRX(CC2520_INS_REGWR | addr);
    CC2520_INS_WR_ARRAY(count, pValues);
    CC2520_SPI_END();
    return s;
}


/***********************************************************************************
* @fn      CC2520_REGWR8
*
* @brief   Write one register byte
*
* @param  uint8_t addr - address
*         uint8_t value
*
* @return  none
*/
void CC2520_REGWR8(uint8_t addr, uint8_t value)
{
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGWR | addr);
    CC2520_SPI_TXRX(value);
    CC2520_SPI_END();
    return;
}


/***********************************************************************************
* @fn      CC2520_REGWR16
*
* @brief   Write two register bytes
*
* @param  uint8_t addr - address
*         uint16_t value
*
* @return  none
*/
void CC2520_REGWR16(uint8_t addr, uint16_t value)
{
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGWR | addr);
    CC2520_SPI_TXRX(LO_UINT16(value));
    CC2520_SPI_TXRX(HI_UINT16(value));
    CC2520_SPI_END();
}


/***********************************************************************************
* @fn      CC2520_REGWR24
*
* @brief   Write three register bytes
*
* @param  uint8_t addr
*         uint32_t value
*
* @return  none
*/
void CC2520_REGWR24(uint8_t addr, uint32_t value)
{
    CC2520_SPI_BEGIN();
    CC2520_SPI_TXRX(CC2520_INS_REGWR | addr);
    CC2520_SPI_TXRX(LO_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(HI_UINT16(LO_UINT32(value)));
    CC2520_SPI_TXRX(LO_UINT16(HI_UINT32(value)));
    CC2520_SPI_END();
}

/** @} */
