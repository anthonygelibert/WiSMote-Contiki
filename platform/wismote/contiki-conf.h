/******************************************************************************
 * Copyright 2010, Plateforme Technologique de Valence                        *
 *                                                                            *
 *    Licensed under the Apache License, Version 2.0 (the "License");         *
 *    you may not use this file except in compliance with the License.        *
 *    You may obtain a copy of the License at                                 *
 *                                                                            *
 *        http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                            *
 *    Unless required by applicable law or agreed to in writing, software     *
 *    distributed under the License is distributed on an "AS IS" BASIS,       *
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *    See the License for the specific language governing permissions and     *
 *    limitations under the License.                                          *
 ******************************************************************************/
/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2010
 * \version 0.0.2
 *
 * This file defines the "include", "typedef", "define", etc... required by the
 * platform.
 */
#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#define CC_CONF_REGISTER_ARGS          1
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1
#define CC_CONF_INLINE                 inline
#define CCIF
#define CLIF

/** CPU target speed in Hz (default value: 2457600uL) */
#define F_CPU 3900000uL
/** Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND 128UL
/** Baud to UBR */
#define BAUD2UBR(baud) ((F_CPU/baud))

typedef unsigned short uip_stats_t;
typedef unsigned long clock_time_t;

#include <msp430f5437.h>
#include "msp430def.h"

#endif /* __CONTIKI_CONF_H__ */
