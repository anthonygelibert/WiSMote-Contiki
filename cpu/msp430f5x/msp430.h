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

#ifndef C_MSP430_H__
#define PUBLIC extern
#else
#define PUBLIC
#endif

#ifdef F_CPU
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_CPU_SPEED F_CPU
#else
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_CPU_SPEED 2457600UL
#endif

#pragma mark LPM Macro

/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_CPUON 0
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM1 1
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM2 2
/** NOT_YET_DOCUMENTED_PTV */
#define MSP430_REQUIRE_LPM3 3
/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * \note We add the LPM4 level... which was not present in sky.
 */
#define MSP430_REQUIRE_LPM4 4


#pragma mark LPM Functions

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @param req NOT_YET_DOCUMENTED_PTV
 */
PUBLIC void msp430_add_lpm_req(int req);

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @param req NOT_YET_DOCUMENTED_PTV
 */
PUBLIC void msp430_remove_lpm_req(int req);

#endif /* __MSP430_H__ */
