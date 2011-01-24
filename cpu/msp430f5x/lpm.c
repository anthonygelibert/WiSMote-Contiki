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

#define __C_LPM_H__
#include "lpm.h"
#include "dco.h"


/** NOT_YET_DOCUMENTED_PTV */
static const int MSP430_REQUIRE_CPUON = 0;
/** NOT_YET_DOCUMENTED_PTV */
static const int MSP430_REQUIRE_LPM1 = 1;
/** NOT_YET_DOCUMENTED_PTV */
static const int MSP430_REQUIRE_LPM2 = 2;
/** NOT_YET_DOCUMENTED_PTV */
static const int MSP430_REQUIRE_LPM3 = 3;

/*---------------------------------------------------------------------------*/
/* add/remove_lpm_req - for requiring a specific LPM mode. currently Contiki */
/* jumps to LPM3 to save power, but DMA will not work if DCO is not clocked  */
/* so some modules might need to enter their LPM requirements                */
/* NOTE: currently only works with LPM1 (e.g. DCO) requirements.             */
/*---------------------------------------------------------------------------*/
void msp430_add_lpm_req(int req)
{
    if (req <= MSP430_REQUIRE_LPM1)
    {
        msp430_dco_required_inc();
    }
}

void msp430_remove_lpm_req(int req)
{
    if (req <= MSP430_REQUIRE_LPM1)
    {
        msp430_dco_required_dec();
    }
}
