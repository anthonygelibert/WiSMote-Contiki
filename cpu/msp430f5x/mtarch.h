/*
 * Copyright (c) 2011, Plateforme Technologique de Valence.
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
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
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
/**
 * \author Anthony Gelibert and Fabien Rey
 * \date Jan 24, 2010
 * \version 0.0.1
 */

#ifndef __MTARCH_H__
#define __MTARCH_H__

/** Size of the stack. */
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
int
mtarch_stack_usage(struct mt_thread *t);

#endif /* __MTARCH_H__ */

