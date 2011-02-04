/* NOT_YET_DOCUMENTED_PTV: Licence */

#include "contiki-conf.h"
#include "sys/clock.h"
#include "signal.h"

/* NOT_YET_DOCUMENTED_PTV */
extern void
clock_platform_init(void);

/* TODO_PTV msp430/clock.c (interrupt) */

void
clock_init(void)
{
  dint();

  clock_platform_init();

  /* Set XT1 On */
  UCSCTL6 &= ~XT1OFF;
  /* Max drive strength, adjust according to crystal frequency. LFXT1 HF mode */
  UCSCTL6 |= XT1DRIVE_2 | XTS | XT2OFF;

  /* Loop while oscillator fault flag. */
  do {
    /* Clear XT2,XT1,DCO fault flags */
    UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | XT1HFOFFG | DCOFFG);
    /* Clear fault flags */
    SFRIFG1 &= ~OFIFG;
  }
  while (SFRIFG1 & OFIFG);

  /* MCLCK = XT1 , SMCLK = XT1 , ACLK = XT1 */
  UCSCTL4 = SELA__XT1CLK | SELS__XT1CLK | SELM__XT1CLK;
  /* Set Clock divider 4 -> Clock = 4MHz */
  UCSCTL5 |= DIVA__4 | DIVS__4 | DIVM__4;
  eint();
}

void
clock_delay(unsigned int i)
{
  asm("add #-1, r15");
  asm("jnz $-2");

}
