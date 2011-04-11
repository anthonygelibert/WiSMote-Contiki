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

#ifndef __CC2520_CONST_H__
#define __CC2520_CONST_H__

/*******************************************************************************
 * CONSTANTS AND DEFINES
 */
/* ----- TXPOWER values ----- */
/** Gives ~ -3.1 dBm TX output power. */
#define CC2520_TXPOWER_MIN_4_DBM        0x88
/** Gives ~ 0.3 dBm TX output power. */
#define CC2520_TXPOWER_0_DBM            0x32
/** Gives ~ 4.6 dBm TX output power. */
#define CC2520_TXPOWER_4_DBM            0xF7

/* ----- TXCTRL values ----- */
/** Recommended value for -30-- °C. */
#define CC2520_TXCTRL_MINUS_30_C 0x13
/** Recommended value for -30 < x < -10 °C. */
#define CC2520_TXCTRL_MINUS_10_C 0xAB
/** Recommended value for +10 °C. */
#define CC2520_TXCTRL_PLUS_10_C 0xF2
/** Recommended value for +30 < x < +70  °C. */
#define CC2520_TXCTRL_PLUS_30_C 0xF7
/** Recommended value for +70++ °C. */
#define CC2520_TXCTRL_PLUS_70_C 0x94

/* ------ CCACTRL0 values ----- */
/** Change the default threshold from 108 dB -> 84 dB */
#define CC2520_CCA_THRESHOLD_80_DB 0xF8

/* ------ MDMCTRL0 values ----- */
#define CC2520_MDMCTRL0_15_4_COMPLIANT  0x84
#define CC2520_MDMCTRL0_EXTERNAL_FILTER 0x85

/* ------ MDMCTRL1 values ----- */
#define CC2520_DEFAULT_CORRELATOR_CORRECTION  0x14

/* ------ RXCTRL values -----*/
#define CC2520_RXCTRL_LOWPOWER 0x33
#define CC2520_RXCTRL_NORMAL 0x3F

/* ------ FSCTRL values -----*/
#define CC2520_FSCTRL_NORMAL 0x5A
#define CC2520_FSCTRL_LOWPOWER 0x12
#define CC2520_FSCTRL_HIGHTEMP 0x7B

/* ----- FSCAL1 values ----- */
#define CC2520_VCO_DEFAULT 0x2B

/* ----- AGCCTRL1 values ----- */
#define CC2520_AGC1_DEFAULT 0x11

/* ----- AGCCTRL2 values ----- */
#define CC2520_AGC2_LOW_POWER 0xEB

/* ----- FRMFILT0 values ----- */
#define CC2520_FRAME_FILTER_DISABLED 0x0
#define CC2520_FRAME_FILTER_DEFAULT  0x0D

/* ----- FRMCTRL0 values ----- */
#define CC2520_FRAME_CONTROL_DEFAULT 0x40

/* --- */
#define CC2520_VREG_MAX_STARTUP_TIME        200

#define CC2520_RSSI_OFFSET                76  // dBm (from data sheet)

#define CC2520_MEMORY_SIZE                  0x400

// Startup time values (in microseconds)
#define CC2520_XOSC_MAX_STARTUP_TIME        300
#define CC2520_SRXON_TO_RANDOM_READY_TIME   144

// SREG definitions (BSET/BCLR unsupported)
#define CC2520_CHIPID                  0x040
#define CC2520_VERSION                 0x042

// INSTRUCTION
#define CC2520_INS_SNOP                0x00
#define CC2520_INS_IBUFLD              0x02
#define CC2520_INS_SIBUFEX             0x03
#define CC2520_INS_SSAMPLECCA          0x04
#define CC2520_INS_SRES                0x0F
#define CC2520_INS_MEMRD               0x10
#define CC2520_INS_MEMWR               0x20
#define CC2520_INS_RXBUF               0x30
#define CC2520_INS_RXBUFCP             0x38
#define CC2520_INS_RXBUFMOV            0x32
#define CC2520_INS_TXBUF               0x3A
#define CC2520_INS_TXBUFCP             0x3E
#define CC2520_INS_RANDOM              0x3C
#define CC2520_INS_SXOSCON             0x40
#define CC2520_INS_STXCAL              0x41
#define CC2520_INS_SRXON               0x42
#define CC2520_INS_STXON               0x43
#define CC2520_INS_STXONCCA            0x44
#define CC2520_INS_SRFOFF              0x45
#define CC2520_INS_SXOSCOFF            0x46
#define CC2520_INS_SFLUSHRX            0x47
#define CC2520_INS_SFLUSHTX            0x48
#define CC2520_INS_SACK                0x49
#define CC2520_INS_SACKPEND            0x4A
#define CC2520_INS_SNACK               0x4B
#define CC2520_INS_SRXMASKBITSET       0x4C
#define CC2520_INS_SRXMASKBITCLR       0x4D
#define CC2520_INS_RXMASKAND           0x4E
#define CC2520_INS_RXMASKOR            0x4F
#define CC2520_INS_MEMCP               0x50
#define CC2520_INS_MEMCPR              0x52
#define CC2520_INS_MEMXCP              0x54
#define CC2520_INS_MEMXWR              0x56
#define CC2520_INS_BCLR                0x58
#define CC2520_INS_BSET                0x59
#define CC2520_INS_CTR                 0x60
#define CC2520_INS_CBCMAC              0x64
#define CC2520_INS_UCBCMAC             0x66
#define CC2520_INS_CCM                 0x68
#define CC2520_INS_UCCM                0x6A
#define CC2520_INS_ECB                 0x70
#define CC2520_INS_ECBO                0x72
#define CC2520_INS_ECBX                0x74
#define CC2520_INS_ECBXO               0x76
#define CC2520_INS_INC                 0x78
#define CC2520_INS_ABORT               0x7F
#define CC2520_INS_REGRD               0x80
#define CC2520_INS_REGWR               0xC0

#endif

/** @} */
