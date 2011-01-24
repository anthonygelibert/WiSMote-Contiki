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
 * \version 0.0.1
 */

#ifndef __MSP430DEF_H__
#define __MSP430DEF_H__

#ifndef __C_MSP430DEF_H_
#define PUBLIC extern
#else
#define PUBLIC
#endif

#include <stdint.h>
/* These names are deprecated, use C99 names. */
typedef  uint8_t    u8_t;
typedef uint16_t   u16_t;
typedef uint32_t   u32_t;
typedef  int32_t   s32_t;

/* default DCOSYNCH Period is 30 seconds */
#ifdef DCOSYNCH_CONF_PERIOD
/** NOT_YET_DOCUMENTED_PTV */
#define DCOSYNCH_PERIOD DCOSYNCH_CONF_PERIOD
#else
/** NOT_YET_DOCUMENTED_PTV */
#define DCOSYNCH_PERIOD 30
#endif

/** NOT_YET_DOCUMENTED_PTV */
#define cpu_init() msp430_cpu_init()

/** NOT_YET_DOCUMENTED_PTV */
PUBLIC void msp430_cpu_init(void);
/** NOT_YET_DOCUMENTED_PTV */
PUBLIC void msp430_sync_dco(void);

/** NOT_YET_DOCUMENTED_PTV */
PUBLIC void   *sbrk(int);

/** NOT_YET_DOCUMENTED_PTV */
typedef int spl_t;

/** NOT_YET_DOCUMENTED_PTV */
PUBLIC void    splx_(spl_t);
/** NOT_YET_DOCUMENTED_PTV */
PUBLIC spl_t   splhigh_(void);

/** NOT_YET_DOCUMENTED_PTV */
#define splhigh() splhigh_()

/** NOT_YET_DOCUMENTED_PTV */
#define splx(sr) __asm__ __volatile__("bis %0, r2" : : "r" (sr))

#undef PUBLIC
#endif /* __MSP430DEF_H__ */
