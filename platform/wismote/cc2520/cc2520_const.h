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

#endif

/** @} */
