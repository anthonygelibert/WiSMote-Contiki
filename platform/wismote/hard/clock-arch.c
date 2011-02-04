/** NOT_YET_DOCUMENTED_PTV : Licence */
#include <contiki-conf.h>

#define DEBUG_CLOCK

/**
 * NOT_YET_DOCUMENTED_PTV
 */
void clock_platform_init(void)
{
#ifdef DEBUG_CLOCK
  P4DIR |= 0x80;
  P4SEL |= 0x80;
#endif
  /* XT1 */
  P7SEL |= BIT0 + BIT1;
}
