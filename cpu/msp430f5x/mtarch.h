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

#ifndef __MTARCH_H__
#define __MTARCH_H__

#ifndef __C_MTARCH_H_
#define PUBLIC extern
#else
#define PUBLIC
#endif

/** NOT_YET_DOCUMENTED_PTV */
#define MTARCH_STACKSIZE 128

/** NOT_YET_DOCUMENTED_PTV */
struct mtarch_thread
{
    /** NOT_YET_DOCUMENTED_PTV */
    unsigned short stack[MTARCH_STACKSIZE];
    /** NOT_YET_DOCUMENTED_PTV */
    unsigned short *sp;
    /** NOT_YET_DOCUMENTED_PTV */
    void *data;
    /** NOT_YET_DOCUMENTED_PTV */
    void *function;
};

/** NOT_YET_DOCUMENTED_PTV */
struct mt_thread;

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * @param t NOT_YET_DOCUMENTED_PTV
 * @return NOT_YET_DOCUMENTED_PTV
 */
PUBLIC int mtarch_stack_usage(struct mt_thread *t);

#undef PUBLIC
#endif /* __MTARCH_H__ */

