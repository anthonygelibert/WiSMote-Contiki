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

#ifndef __RTIMER_ARCH_H__
#define __RTIMER_ARCH_H__

#include <io.h>
#include "sys/rtimer.h"

#ifndef __C_RTIMER_ARCH_H__
#define PUBLIC extern
#else
#define PUBLIC
#endif

/** NOT_YET_DOCUMENTED_PTV
 *  \note Ask to NICOLAS.
 */
#define RTIMER_ARCH_SECOND (4096U*8)

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @return NOT_YET_DOCUMENTED_PTV
 */
PUBLIC rtimer_clock_t rtimer_arch_now(void);

#undef PUBLIC
#endif /* __RTIMER_ARCH_H__ */
