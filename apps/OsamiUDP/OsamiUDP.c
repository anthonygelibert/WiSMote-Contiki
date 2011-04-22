/**
 * \file
 *         OsamiUDP Example - Program.
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

/* From MSP430-GCC */
#include <string.h>

/* From CONTIKI */
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "dev/sht11-sensor.h"

/* From this example */
#include "SensorDemoConfig.h"

/** Enable debug. */
#define DEBUG 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((u8_t *)addr)[0], ((u8_t *)addr)[1], ((u8_t *)addr)[2], ((u8_t *)addr)[3], ((u8_t *)addr)[4], ((u8_t *)addr)[5], ((u8_t *)addr)[6], ((u8_t *)addr)[7], ((u8_t *)addr)[8], ((u8_t *)addr)[9], ((u8_t *)addr)[10], ((u8_t *)addr)[11], ((u8_t *)addr)[12], ((u8_t *)addr)[13], ((u8_t *)addr)[14], ((u8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF(" %02x:%02x:%02x:%02x:%02x:%02x ",(lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3],(lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

static uip_ipaddr_t ownaddr;
static uip_ipaddr_t destaddr;
static struct uip_udp_conn *udpconn_in_discovery;
static struct uip_udp_conn *udpconn_out_data;
static struct uip_udp_conn *udpconn_out_discovery;

/*---------------------------------------------------------------------------*/

/**
 * Translate an unsigned 8 bits value (0 ~> 255) to a decimal string.
 *
 * \param u8Data  8 bits value.
 * \param pString Target string.
 */
static void Print8BitsToDecString(u8_t u8Data, u8_t *pString)
{
    /*
     * Reminder:
     * value between 0 ~> 9 + '0' == ASCII character between '0' ~> '9'.
     */
    *pString++ = (u8Data / 100) + '0';
    u8Data %= 100;
    *pString++ = (u8Data / 10) + '0';
    *pString++ = u8Data % 10 + '0';
}

/*---------------------------------------------------------------------------*/

/**
 * Translate an unsigned 16 bits (0 ~> 65535) value to a decimal string.
 *
 * \param u16Data  16 bits value.
 * \param pString Target string.
 */
static void Print16BitsToDecString(u16_t u16Data, u8_t *pString)
{
  /*
   * Reminder:
   * value between 0 ~> 9 + '0' == ASCII character between '0' ~> '9'.
   */
  *pString++ = (u16Data / 10000) + '0';
  u16Data %= 10000;
  *pString++ = (u16Data / 1000) + '0';
  u16Data %= 1000;
  *pString++ = (u16Data / 100) + '0';
  u16Data %= 100;
  *pString++ = (u16Data / 10) + '0';
  *pString++ = u16Data % 10 + '0';
}

/*
// Will be used later - please don't remove.
static void Print16BitsToHexString(const u16_t u16Data, u8_t *pString)
{
    int   i;
    u8_t  u8Nybble;

    for (i = 12; i >= 0; i -= 4)
    {
        u8Nybble = (u8_t)((u16Data >> i) & 0x0f);
        u8Nybble += '0';
        if (u8Nybble > '9')
            u8Nybble += 'A' - '9' - 1;

        *pString++ = u8Nybble;
    }
}*/

/*---------------------------------------------------------------------------*/

/**
 * Translate an IPv4 address to a decimal string.
 *
 * \param psSock The IPv4 address.
 * \param pString Target string.
 * \return 15
 */
static int PrintAddr(const u8_t *psSock, u8_t *pString)
{
    int i;
    for (i = 0; i < 4; i++)
    {
      Print8BitsToDecString(psSock[i], pString);
        pString += 3;
        if (i != 3)
            *pString++ = '.';
    }
    return (4*3 + 3*1);
}

/*
// IPv6 version, will be used later - please don't remove.
static int PrintAddr(const u16_t *psSock, u8_t *pString)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        Print16BitsToHexString(psSock[i], pString);
        pString += 4;
        if (i != 7)
            *pString++ = ':';
    }
    return (8*4 + 7*1);
}
*/

/*---------------------------------------------------------------------------*/

/**
 * Metadata message "handler".
 */
static void metadata(void)
{
    u8_t buf[200];
    u8_t * pu8Payload;
    const u8_t * sOwnAddr = ownaddr.u8;
    const u8_t * sAddr = destaddr.u8;
//    u16_t * sOwnAddr = (u16_t *)(&uip_netif_physical_if.addresses[0].ipaddr);
//    u16_t * sAddr = (u16_t *)(&udpconn_out_data->ripaddr);

    PRINTF("Sender sending to: ");
    PRINT6ADDR(sAddr);
    PRINTF("\n");

    // Check for GETMETADATA message.
    if (!(   ((char *)uip_appdata)[0] == DEVICE_PROTOCOL_VERSION_1
          && ((char *)uip_appdata)[1] == DEVICE_PROTOCOL_VERSION_2
          && ((char *)uip_appdata)[2] == DEVICE_GETMETADATA_PREFIX
         ))
    {
        // Not a GETMETADATA message
        return; // THIS RETURNS !
    }

    // Send METADATA message.
    pu8Payload = buf;

#if USE_MULTICAST_MESSAGE
    /* START MULTICAST MESSAGE */
    /* Prefix */
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_1;
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_2;
    /* From */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* To */
    pu8Payload += PrintAddr(sAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Port */
    Print16BitsToDecString(DISCOVERY_PORT, pu8Payload);
    pu8Payload += 5;
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
#endif

    /* START DISCOVERY MESSAGE */
    /* Prefix */
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_1;
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_2;
    /* METADATA */
    /* EVENT */
    *pu8Payload++ = DEVICE_METADATA_PREFIX;
    /* id : MAC address */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* Pairs Key/Value */
    /* Key 1 */
    *pu8Payload++ = 'T';
    *pu8Payload++ = 'e';
    *pu8Payload++ = 'm';
    *pu8Payload++ = 'p';
    *pu8Payload++ = '1';
    /* Separator Key/Value */
    *pu8Payload++ = DEVICE_METADATA_KV_SEPARATOR;
    /* Value 1 */
    *pu8Payload++ = 'S';
    *pu8Payload++ = 'H';
    *pu8Payload++ = 'T';
    *pu8Payload++ = '1';
    *pu8Payload++ = 'x';
    /* Separator Pairs {Key/Value} */
    *pu8Payload++ = DEVICE_METADATA_FIELD_SEPARATOR;

    /* Key 2 */
    *pu8Payload++ = 'H';
    *pu8Payload++ = 'y';
    *pu8Payload++ = 'd';
    *pu8Payload++ = 'r';
    *pu8Payload++ = 'o';
    *pu8Payload++ = '1';
    /* Separator Key/Value */
    *pu8Payload++ = DEVICE_METADATA_KV_SEPARATOR;
    /* Value 2 */
    *pu8Payload++ = 'S';
    *pu8Payload++ = 'H';
    *pu8Payload++ = 'T';
    *pu8Payload++ = '1';
    *pu8Payload++ = 'x';
    /* Separator Pairs {Key/Value} */
    *pu8Payload++ = DEVICE_METADATA_FIELD_SEPARATOR;

    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* END DISCOVERY MESSAGE */

#if USE_MULTICAST_MESSAGE
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
    /* END MULTICAST MESSAGE */
#endif

    *pu8Payload++ = 0;

    uip_udp_packet_send(udpconn_out_discovery, buf, (pu8Payload - buf));
}

/*---------------------------------------------------------------------------*/

/**
 * Discovery message "handler".
 */
static void discovery(void)
{
    u8_t buf[200];
    u8_t * pu8Payload;
    const u8_t * sOwnAddr = ownaddr.u8;
    const u8_t * sAddr = destaddr.u8;
//    u16_t * sOwnAddr = (u16_t *)(&uip_netif_physical_if.addresses[0].ipaddr);
//    u16_t * sAddr = (u16_t *)(&udpconn_out_data->ripaddr);

    PRINTF("Sender sending to: ");
    PRINT6ADDR(sAddr);
    PRINTF("\n");

    // Send HELLO message.
    pu8Payload = buf;

#if USE_MULTICAST_MESSAGE
    /* START MULTICAST MESSAGE */
    /* Prefix */
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_1;
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_2;
    /* From */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* To */
    pu8Payload += PrintAddr(sAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Port */
    Print16BitsToDecString(DISCOVERY_PORT, pu8Payload);
    pu8Payload += 5;
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
#endif

    /* START DISCOVERY MESSAGE */
    /* Prefix */
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_1;
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_2;
    /* HELLO */
    *pu8Payload++ = DEVICE_HELLO_PREFIX;
    /* id : MAC address */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* ip : ipv6 address */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    Print16BitsToDecString(DISCOVERY_PORT, pu8Payload);
    pu8Payload += 5;
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* END DISCOVERY MESSAGE */

#if USE_MULTICAST_MESSAGE
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
    /* END MULTICAST MESSAGE */
#endif

    *pu8Payload++ = 0;

    uip_udp_packet_send(udpconn_out_discovery, buf, (pu8Payload - buf));
}

/**
 * Event message "handler".
 */
static void event(void)
{
    u8_t buf[200];
    u8_t * pu8Payload;
    const u8_t * sOwnAddr = ownaddr.u8;
    const u8_t * sAddr = destaddr.u8;
    u16_t tmp, rh, val2;
//    u16_t * sOwnAddr = (u16_t *)(&uip_netif_physical_if.addresses[0].ipaddr);
//    u16_t * sAddr = (u16_t *)(&udpconn_out_data->ripaddr);

    PRINTF("Sender sending to: ");
    PRINT6ADDR(sAddr);
    PRINTF("\n");

    // Send EVENT message containing measurements.
    pu8Payload = buf;

#if USE_MULTICAST_MESSAGE
    /* START MULTICAST MESSAGE */
    /* Prefix */
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_1;
    *pu8Payload++ = MESSAGE_PROTOCOL_VERSION_2;
    /* From */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* To */
    pu8Payload += PrintAddr(sAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Port */
    Print16BitsToDecString(PC_UDP_PORT, pu8Payload);
    pu8Payload += 5;
    /* Separator */
    *pu8Payload++ = MESSAGE_FIELD_SEPARATOR;
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
#endif

    /* START DISCOVERY MESSAGE */
    /* Prefix */
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_1;
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_2;

    /* Enable the sensor */
    SENSORS_ACTIVATE(sht11_sensor);
    /* Read TEMP and HUMIDITY */
    tmp = sht11_sensor.value(SHT11_SENSOR_TEMP);
    if (tmp == -1 || tmp == 0) tmp = 3970; // See below for formula.
    rh = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
    if (rh == -1 || rh == 0) rh = 56; // See below for formula.
    /* Disable the sensor */
    SENSORS_DEACTIVATE(sht11_sensor);

    /* EVENT */
    *pu8Payload++ = DEVICE_EVENT_PREFIX;
    /* id : MAC address */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* Pairs Key/Value */
    /* Key 1 */
    *pu8Payload++ = 'T';
    *pu8Payload++ = 'e';
    *pu8Payload++ = 'm';
    *pu8Payload++ = 'p';
    *pu8Payload++ = '1';
    /* Separator Key/Value */
    *pu8Payload++ = DEVICE_EVENT_KV_SEPARATOR;
    /* Value 1 */
    // T = -39.7 + 0.01 * tmp (14-bit tmp, at 3.5V)
    val2 = ((int)tmp - 3970) / 100;
    PRINTF("T:%d %d\n", tmp, val2);
    if (val2 < 0)
    {
        *pu8Payload++ = '-';
        val2 = -val2;
    }
    Print8BitsToDecString(val2, pu8Payload);
    pu8Payload += 3;

    /* Separator Pairs {Key/Value} */
    *pu8Payload++ = DEVICE_EVENT_FIELD_SEPARATOR;

    u16_t acc1, acc2;
    /* Key 2 */
    *pu8Payload++ = 'H';
    *pu8Payload++ = 'y';
    *pu8Payload++ = 'd';
    *pu8Payload++ = 'r';
    *pu8Payload++ = 'o';
    *pu8Payload++ = '1';
    /* Separator Key/Value */
    *pu8Payload++ = DEVICE_EVENT_KV_SEPARATOR;
    /* Value 2 */
    // RHlinear       = -2.0468 + 0.0367 * val - 1.5955e-6 * val * val (12-bit val)
    // 256 * RHLinear = -523.98 + 9.3952  * val - 4.0845e-4 * val * val
    // 256 * RHLinear ~ -524    + 9.4     * val - val * val / 2448;
    acc1 = ((u32_t)rh * 940) / 100;
    acc2 = ((u32_t)rh * rh) / 2448;
    val2 = (u16_t)acc1 - (u16_t) acc2 - 524;
    val2 >>= 8;
    PRINTF("RH:%d %u %u %d\n", rh, acc1, acc2, val2);
    Print8BitsToDecString(val2, pu8Payload);
    pu8Payload += 3;

    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* END DISCOVERY MESSAGE */

#if USE_MULTICAST_MESSAGE
    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
    /* END MULTICAST MESSAGE */
#endif

    *pu8Payload++ = 0;

    PRINTF("%s\n", buf);

    uip_udp_packet_send(udpconn_out_data, buf, (pu8Payload - buf));
}

PROCESS(OsamiUDP_process, "OSAMI UDP");
AUTOSTART_PROCESSES(&OsamiUDP_process);

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(OsamiUDP_process, ev, data)
{
    static struct etimer data_timer;
    static struct etimer discovery_timer;

    PROCESS_BEGIN();

    PRINTF("Process test UDP sender started\n");
    PRINTF("Local IPv6 address: ");
    uip_ipaddr(&ownaddr, 169, 254, 5, 210);
    uip_sethostaddr(&ownaddr);
    uip_ipaddr(&destaddr, 169, 254, 5, 212);
    PRINTF("\n");

    /* new connection with remote host */
    udpconn_out_data = uip_udp_new(&destaddr, uip_htons(DATA_PORT));
    udpconn_out_discovery = uip_udp_new(&destaddr, uip_htons(DISCOVERY_PORT));

    udpconn_in_discovery = udp_new(&uip_all_zeroes_addr, 0, NULL);
    udp_bind(udpconn_in_discovery,uip_htons(DISCOVERY_PORT));

    PRINTF("Created connection with remote peer ");

    etimer_set(&discovery_timer, 10 * CLOCK_SECOND);
    etimer_set(&data_timer, 3 * CLOCK_SECOND);

    while(1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&discovery_timer) || etimer_expired(&data_timer) || ev == tcpip_event);
        if (uip_newdata()) {
            metadata();
        }
        if (etimer_expired(&discovery_timer))
        {
            discovery();
            etimer_restart(&discovery_timer);
        }
        else // if (etimer_expired(&data_timer))
        {
            event();
            etimer_restart(&data_timer);
        }
    }

    PROCESS_END();
}

/** @} */
