/**
 * \addtogroup wismote
 * @{
 */

/**
 * \file
 *         Button sensor on Wismote.
 * \author
 *         Anthony Gelibert <anthony.gelibert@lcis.grenoble-inp.fr>
 * \date
 *         March 21, 2011
 */

/*
 * Copyright (c) 2011, LCIS/CTSYS.
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
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS "AS IS" AND
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

#ifndef __DIAG_H__
#define __DIAG_H__

/* From CONTIKI */
#include "contiki.h"

/* Export the name of the process. */
PROCESS_NAME(diagnostic_process);

/** Diagnostic will use an ASCII output. */
#define ASCII_OUTPUT 1
/** Diagnostic will use an XML output. */
#define XML_OUTPUT   2
/** Diagnostic will use a JSON output. */
#define JSON_OUTPUT  3

#ifndef DIAGNOSTIC_CONF_OUTPUT
#define DIAGNOSTIC_OUTPUT ASCII_OUTPUT
#else
#define DIAGNOSTIC_OUTPUT (DIAGNOSTIC_CONF_OUTPUT)
#endif

#if ((DIAGNOSTIC_OUTPUT != ASCII_OUTPUT) && (DIAGNOSTIC_OUTPUT != XML_OUTPUT) && (DIAGNOSTIC_OUTPUT != JSON_OUTPUT))
#error "The specified output type is unknown !"
#endif

/** Define the type "command handler". */
typedef char * (*cmdHandler)(void);

/** A command. */
struct command {
    /** Name. */
    char name[10];
    /** Handler. */
    cmdHandler handler;
};

/** Export the command. */
#define COMMAND_NAME(sym) extern struct command sym

/** Define a new command. */
#define COMMAND(sym, name,handler)  struct command sym = { name , handler }

#endif

/** @} **/
