#include <spi.h>
#include "contiki-conf.h"

#define SPI_MOSI BIT1
#define SPI_MISO BIT2
#define SPI_CLK  BIT3

#define SPI_Px_SEL P3SEL
#define SPI_Px_DIR P3DIR

unsigned char spi_busy = 0;

void
spi_init(void)
{
  // Put state machine in reset
  UCB0CTL1 |= UCSWRST;
  // Select SMCLK
  UCB0CTL1 = UCSSEL__SMCLK;
  UCB0CTL0 |=  UCCKPH | UCSYNC | UCMSB | UCMST; // 3-pin, 8-bit SPI master, rising edge capture

  // 16 bit baud rate register
  UCB0BR0 = 0x00;                         // MSB => 0
  UCB0BR1 = 0x08;                         // LSB => SMCLK / (UCxxBR0 + UCxxBR1 � 256)

  // Set MOSI and SCLK as OUT and MISO as IN ports
  SPI_Px_SEL |= (SPI_MOSI | SPI_MISO | SPI_CLK); // Port3 = SPI peripheral
  SPI_Px_DIR |= (SPI_MOSI | SPI_CLK);    // MOSI and SCLK as Output
  SPI_Px_DIR &= ~SPI_MISO;                     // Don't forget to configure MISO as Input
  UCB0IE |= UCTXIE | UCRXIE;              // Enable interrupt for RX and TX SPI buffer
  UCB0CTL1 &= ~UCSWRST;                   // Initialize USCI state machine
}
