#include <contiki-conf.h>

void clock_platform_init(void)
{
  P4DIR |= 0x80;
  P4SEL |= 0x80;

  P7SEL |= BIT0 + BIT1;
}
