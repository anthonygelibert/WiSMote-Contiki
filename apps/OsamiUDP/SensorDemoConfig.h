/**
 * \file
 *         OsamiUDP Example - Constants.
 * \author
 *         Lionel Debroux <lionel.debroux@lcis.grenoble-inp.fr>
 * \date
 *         March 31, 2011
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

#ifndef __SENSORDEMOCONFIG_H__
#define __SENSORDEMOCONFIG_H__

/*****************************************************************************/
/***        Macro Definitions                                              ***/
/*****************************************************************************/

/* Special bytes, keep in sync with Java classes ! */
#define MESSAGE_FIELD_SEPARATOR         ';'
#define MESSAGE_WRAPPER                 '$'
#define MESSAGE_PROTOCOL_VERSION_1      'M'
#define MESSAGE_PROTOCOL_VERSION_2      '1'

#define DEVICE_FIELD_SEPARATOR          '!'
#define DEVICE_PROTOCOL_VERSION_1       'O'
#define DEVICE_PROTOCOL_VERSION_2       '1'
#define DEVICE_HELLO_PREFIX             'H'
#define DEVICE_PROBE_PREFIX             'P'
#define DEVICE_BYE_PREFIX               'B'
#define DEVICE_GETMETADATA_PREFIX       'G'
#define DEVICE_METADATA_PREFIX          'M'
#define DEVICE_EVENT_PREFIX             'E'

#define DEVICE_EVENT_KV_SEPARATOR       '|'
#define DEVICE_EVENT_FIELD_SEPARATOR    ';'

#define DEVICE_METADATA_KV_SEPARATOR    '|'
#define DEVICE_METADATA_FIELD_SEPARATOR ';'

#define DATA_PORT                       61615
#define DISCOVERY_PORT                  61616

#endif  /* __SENSORDEMOCONFIG_H__ */

/** @} */

