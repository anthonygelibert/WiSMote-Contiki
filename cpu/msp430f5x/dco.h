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
 * \author Anthony Gelibert
 * \date Jan 24, 2010
 * \version 0.0.1
 */

#ifndef __DCO_H__
#define __DCO_H__

#ifndef __C_DCO_H__
#define PUBLIC extern
#else
#define PUBLIC
#endif

/** Initialize DCO. */
PUBLIC void msp430_init_dco(void);

/**
 * Synchronize the DCO.
 *
 * \note this code will always start the TimerB if not already started
 */
PUBLIC void msp430_sync_dco(void);

/** Increment the msp430_dco_required value. */
PUBLIC void msp430_dco_required_inc(void);

/** Decrement the msp430_dco_required value. */
PUBLIC void msp430_dco_required_dec(void);

/** Reset the msp430_dco_required value. */
PUBLIC void msp430_dco_required_reset(void);

#undef PUBLIC
#endif
