

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include <string.h>

#include "dev/sht11-sensor.h"

#include "SensorDemoConfig.h"


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

#define UDP_NB 4
#define UDP_MAX_DATA_LEN 200

/*clock_delay(65535);*/

static struct uip_udp_conn *udpconn_discovery;
static struct uip_udp_conn *udpconn_data;

static void Print8BitsToDecString(u8_t u8Data, u8_t *pString)
{
    *pString++ = (u8Data / 100) + '0';
    u8Data %= 100;
    *pString++ = (u8Data / 10) + '0';
    *pString++ = u8Data % 10 + '0';
}

static void Print16BitsToDecString(u16_t u16Data, u8_t *pString)
{
    *pString++ = (u16Data / 10000) + '0';
    u16Data %= 10000;
    *pString++ = (u16Data / 1000) + '0';
    u16Data %= 1000;
    *pString++ = (u16Data / 100) + '0';
    u16Data %= 100;
    *pString++ = (u16Data / 10) + '0';
    *pString++ = u16Data % 10 + '0';
}

/*static void Print8BitsToHexString(const u8_t u16Data, u8_t *pString)
{
    u8_t u8Nybble;

    u8Nybble = (u8_t)((u16Data >> 4) & 0x0f);
    u8Nybble += '0';
    if (u8Nybble > '9')
        u8Nybble += 'A' - '9' - 1;
    *pString++ = u8Nybble;

    u8Nybble = (u8_t)((u16Data >> 0) & 0x0f);
    u8Nybble += '0';
    if (u8Nybble > '9')
        u8Nybble += 'A' - '9' - 1;
    *pString++ = u8Nybble;
}

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
static void discovery(void)
{
    u8_t buf[200];
    u8_t * pu8Payload;
    const u8_t * sOwnAddr = (u8_t [4]){172, 28, 1, 100};
    const u8_t * sAddr = (u8_t [4]){172, 28, 1, 198};
//    u16_t * sOwnAddr = (u16_t *)(&uip_netif_physical_if.addresses[0].ipaddr);
//    u16_t * sAddr = (u16_t *)(&udpconn_data->ripaddr);

    PRINTF("Sender sending to: ");
    PRINT6ADDR(sAddr);
    PRINTF("\n");

    // Send HELLO message.
    pu8Payload = buf;

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

    /* START DISCOVERY MESSAGE */
    /* Prefix */
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_1;
    *pu8Payload++ = DEVICE_PROTOCOL_VERSION_2;
    /* Lettre HELLO */
    *pu8Payload++ = DEVICE_HELLO_PREFIX;
    /* id : MAC Adress */
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

    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
    /* END MULTICAST MESSAGE */

    *pu8Payload++ = 0;

//    PRINTF("%s\n", buf);

    uip_udp_packet_send(udpconn_discovery, buf, (pu8Payload - buf));
}


static void event(void)
{
    u8_t buf[200];
    u8_t * pu8Payload;
    const u8_t * sOwnAddr = (u8_t [4]){172, 28, 1, 100};
    const u8_t * sAddr = (u8_t [4]){172, 28, 1, 198};
    u16_t tmp, rh, val2;
//    u16_t * sOwnAddr = (u16_t *)(&uip_netif_physical_if.addresses[0].ipaddr);
//    u16_t * sAddr = (u16_t *)(&udpconn_data->ripaddr);

    PRINTF("Sender sending to: ");
    PRINT6ADDR(sAddr);
    PRINTF("\n");

    // Send EVENT message containing measurements.
    pu8Payload = buf;

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

    /* Lettre Send Data*/
    *pu8Payload++ = DEVICE_EVENT_PREFIX;
    /* id : MAC Adress */
    pu8Payload += PrintAddr(sOwnAddr, pu8Payload);
    /* Separator */
    *pu8Payload++ = DEVICE_FIELD_SEPARATOR;
    /* Pairs Key/Value */
    /* Key 1 */
    *pu8Payload++ = 'T';
    *pu8Payload++ = 'e';
    *pu8Payload++ = 'm';
    *pu8Payload++ = 'p';
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

    /* Payload separator */
    *pu8Payload++ = MESSAGE_WRAPPER;
    /* END MULTICAST MESSAGE */

    *pu8Payload++ = 0;

    PRINTF("%s\n", buf);

    uip_udp_packet_send(udpconn_data, buf, (pu8Payload - buf));
}




PROCESS(OsamiUDP_process, "OSAMI UDP");
AUTOSTART_PROCESSES(&OsamiUDP_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(OsamiUDP_process, ev, data)
{
    //static uip_ipaddr_t ownaddr;
    static uip_ipaddr_t ipaddr;
    static struct etimer data_timer;
    static struct etimer discovery_timer;

    PROCESS_BEGIN();

    PRINTF("Process test UDP sender started\n");

    PRINTF("Local IPv6 address: ");
//    PRINT6ADDR(&uip_netif_physical_if.addresses[0].ipaddr);
//    uip_ip6addr(&ownaddr,0xfd04,0x0bd3,0x80e8,0x0001,0x0215,0x8D00,0x0012,0x3456);
    uip_ipaddr(&ipaddr, 172, 28, 1, 198);

    PRINTF("\n");

//    leds_toggle(LEDS_ALL);

//    uip_ip6addr(&ipaddr,0xfe80,0x0000,0x0000,0x0000,0x0012,0x13ff,0xfe14,0x1516);
//    uip_ip6addr(&ipaddr,0xfd04,0x0bd3,0x80e8,0x0001,0x0000,0x0000,0x0000,0x0001);

    /* new connection with remote host */
    udpconn_data = uip_udp_new(&ipaddr, uip_htons(PC_UDP_PORT));
    udpconn_discovery = uip_udp_new(&ipaddr, uip_htons(DISCOVERY_PORT));


    PRINTF("Created connection with remote peer ");
//    PRINT6ADDR(&udpconn_data->ripaddr);

    etimer_set(&discovery_timer, 10 * CLOCK_SECOND);
    etimer_set(&data_timer, 3 * CLOCK_SECOND);

    while(1)
    {
//        u32_t val = (uint16_t)3521 * (uint16_t)3467;
//        u32_t val2 = (val * 6553) + val >> 1;
//        u32_t val3 = val2 >>
//        PRINTF("%lu %lu"
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&discovery_timer) || etimer_expired(&data_timer));
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
