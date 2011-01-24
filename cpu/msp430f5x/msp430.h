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
 */

#ifndef __MSP430_H__
#define __MSP430_H__

#include "contiki-conf.h"
#include "dco.h"
#include "lpm.h"
#include "types.h"

#ifndef __C_MSP430_H__
#define PUBLIC extern
#else
#define PUBLIC
#endif

#ifdef F_CPU
/** CPU speed of MSP430 (in MHz) (F_CPU macro). */
#define MSP430_CPU_SPEED F_CPU
#else
/** CPU speed of MSP430 (in MHz) (F_CPU macro). */
#define MSP430_CPU_SPEED 2457600UL
#endif

/** Permits to mask the real "cpu_init" function. */
#define cpu_init() msp430_cpu_init()

/** Initialize the MSP430 CPU.
 *
 *  This function initializes :
 *  <ol>
 *      <li>WatchDog</li>
 *      <li>Ports</li>
 *      <li>DCO</li>
 *  </ol>
 */
PUBLIC void msp430_cpu_init(void);

#undef PUBLIC
#endif /* __MSP430_H__ */
