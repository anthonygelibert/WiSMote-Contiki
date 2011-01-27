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

#define __C_MTARCH_H_
#include "mtarch.h"
#include "sys/mt.h"

/** NOT_YET_DOCUMENTED_PTV */
static unsigned short *sptmp;
/** NOT_YET_DOCUMENTED_PTV */
static struct mtarch_thread *running;

/** NOT_YET_DOCUMENTED_PTV */
static void
mtarch_wrapper(void);
/** NOT_YET_DOCUMENTED_PTV */
static void
sw(void);

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_init(void)
{
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_start(struct mtarch_thread *t, void
(*function)(void *), void *data)
{
  int i;
  for (i = 0; i < MTARCH_STACKSIZE; ++i) {
    t->stack[i] = i;
  }

  t->sp = &t->stack[MTARCH_STACKSIZE - 1];

  *t->sp = (unsigned short) mt_exit;
  --t->sp;

  *t->sp = (unsigned short) mtarch_wrapper;
  --t->sp;

  /* Space for registers. */
  t->sp -= 11;

  /* Store function and argument (used in mtarch_wrapper) */
  t->data = data;
  t->function = function;
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_exec(struct mtarch_thread *t)
{
  running = t;
  sw();
  running = NULL;
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_remove(void)
{
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_yield(void)
{
  sw();
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_pstop(void)
{
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_pstart(void)
{
}

/** NOT_YET_DOCUMENTED_PTV */
void
mtarch_stop(struct mtarch_thread *thread)
{
}

/** NOT_YET_DOCUMENTED_PTV */
int
mtarch_stack_usage(struct mt_thread *t)
{
  int i;

  for (i = 0; i < MTARCH_STACKSIZE; ++i) {
    if (t->thread.stack[i] != (unsigned short) i) {
      return MTARCH_STACKSIZE - i;
    }
  }
  return MTARCH_STACKSIZE;
}

/** NOT_YET_DOCUMENTED_PTV */
static void
mtarch_wrapper(void)
{
  /* Call thread function with argument */
  ((void
  (*)(void *)) running->function)((void*) running->data);
}

/** NOT_YET_DOCUMENTED_PTV */
static void
sw(void)
{

  sptmp = running->sp;

  __asm__("push r4");
  __asm__("push r5");
  __asm__("push r6");
  __asm__("push r7");
  __asm__("push r8");
  __asm__("push r9");
  __asm__("push r10");
  __asm__("push r11");
  __asm__("push r12");
  __asm__("push r13");
  __asm__("push r14");
  __asm__("push r15");

  __asm__("mov.w r1,%0" : "=r" (running->sp));
  __asm__("mov.w %0,r1" : : "m" (sptmp));

  __asm__("pop r15");
  __asm__("pop r14");
  __asm__("pop r13");
  __asm__("pop r12");
  __asm__("pop r11");
  __asm__("pop r10");
  __asm__("pop r9");
  __asm__("pop r8");
  __asm__("pop r7");
  __asm__("pop r6");
  __asm__("pop r5");
  __asm__("pop r4");
}
