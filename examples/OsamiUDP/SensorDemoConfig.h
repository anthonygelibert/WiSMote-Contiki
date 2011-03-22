#ifndef __SENSORDEMOCONFIG_H__
#define __SENSORDEMOCONFIG_H__

//****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

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

#define PC_UDP_PORT 61615
#define DISCOVERY_PORT 61616

#endif  /* __SENSORDEMOCONFIG_H__ */

