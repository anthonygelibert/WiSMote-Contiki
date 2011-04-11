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

#ifndef __CC2520_UTIL_H__
#define __CC2520_UTIL_H__

#include "contiki-conf.h"

/** Get the high-part of a 16 bits variable */
#define HI_UINT16(a) (((uint16_t)(a) >> 8) & 0xFF)
/** Get the low-part of a 16 bits variable */
#define LO_UINT16(a) ((uint16_t)(a) & 0xFF)

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
uint8_t CC2520_MEMWR(uint16_t addr, uint16_t count, uint8_t  *pData);

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
uint8_t CC2520_MEMWR8(uint16_t addr, uint8_t value);

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
uint8_t CC2520_MEMWR16(uint16_t addr, uint16_t value);

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
uint8_t CC2520_MEMWR24(uint16_t addr, uint32_t value);

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
uint8_t CC2520_MEMRD(uint16_t addr, uint16_t count, uint8_t  *pData);

/******************************************************************************
* @fn      CC2520_MEMRD8
*
* @brief   Read memory 8 bits
*
* @param   uint16_t addr
*
* @return  uint8_t - result
*/
uint8_t CC2520_MEMRD8(uint16_t addr);

/*******************************************************************************
* @fn      CC2520_MEMRD16
*
* @brief   Read memory 16 bits
*
* @param   uint16_t addr
*
* @return  uint16_t - result
*/
uint16_t CC2520_MEMRD16(uint16_t addr);

/*******************************************************************************
* @fn      CC2520_MEMRD24
*
* @brief   Read memory 24 bits
*
* @param   uint16_t addr
*
* @return  uint32_t - result
*/
uint32_t CC2520_MEMRD24(uint16_t addr);

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
uint8_t CC2520_REGRD(uint8_t addr, uint8_t count, uint8_t  *pValues);

/***********************************************************************************
* @fn      CC2520_REGRD8
*
* @brief   Read one register byte
*
* @param  uint8_t addr - address
*
* @return  uint8_t - result
*/
uint8_t CC2520_REGRD8(uint8_t addr);

/***********************************************************************************
* @fn      CC2520_REGRD16
*
* @brief   Read two register bytes
*
* @param  uint8_t addr - address
*
* @return  uint16_t - result
*/
uint16_t CC2520_REGRD16(uint8_t addr);

/***********************************************************************************
* @fn      CC2520_REGRD24
*
* @brief   Read three register bytes
*
* @param  uint8_t addr - address
*
* @return  uint32_t - result
*/
uint32_t CC2520_REGRD24(uint8_t addr);
#endif

/** @} */
