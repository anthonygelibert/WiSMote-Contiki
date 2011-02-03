/* NOT_YET_DOCUMENTED_PTV: Licence */

#include "contiki-conf.h"
#include "sys/clock.h"
#include "signal.h"

/* TODO_PTV msp430/clock.c (interrupt) */

void
clock_init(void)
{
   dint();

   /* XXX_PTV : In platform --- */
   P4DIR |= 0x80;
   P4SEL |= 0x80;

   // Activate XT1
   P7SEL |= BIT0 + BIT1;

   /* ---- */

   // Set XT1 On
   UCSCTL6 &= ~XT1OFF;
   // Max drive strength, adjust according to crystal frequency. LFXT1 HF mode
   UCSCTL6 |= XT1DRIVE_2 + XTS + XT2OFF;

   do
     {
       UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
       // Clear XT2,XT1,DCO fault flags
       SFRIFG1 &= ~OFIFG; // Clear fault flags
     }
   while (SFRIFG1 & OFIFG); // Test oscillator fault flag

   UCSCTL4 = SELA_0 + SELS_0 + SELM_0; // MCLCK = XT1 , SMCLK = XT1 , ACLK = XT1
   UCSCTL5 |= DIVA__4 + DIVS__4 + DIVM__4;// Set Clock divider 4 -> Clock = 4MHz
   eint();
}

void
clock_delay(unsigned int i)
{
  asm("add #-1, r15");
  asm("jnz $-2");

}
