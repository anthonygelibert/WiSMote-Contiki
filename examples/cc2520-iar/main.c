
#include "hal_cc2520.h"
#include "basic_rf.h"


/***********************************************************************************
* CONSTANTS AND DEFINES
*/
#define RECEIVER_MODE     1
#define RECEIVER_ADDR     0x01
#define TRANSMITTER_MODE  2
#define TRANSMITTER_ADDR  0x02
#define NODE_MODE         RECEIVER_MODE
#define CHANNEL_SELECTED  20
#define PANID_SELECTED    0xbabe

// Leds ==> P2.4, P8.6, P5.2
#define LED_DIR()     P2DIR |= BIT4;  P8DIR |= BIT6;  P5DIR |= BIT2;  
#define LED1_ON()     P2OUT &= ~BIT4;
#define LED1_OFF()    P2OUT |= BIT4;
#define LED2_ON()     P8OUT &= ~BIT6;
#define LED2_OFF()    P8OUT |= BIT6;
#define LED3_ON()     P5OUT &= ~BIT2;
#define LED3_OFF()    P5OUT |= BIT2;

/***********************************************************************************
* LOCAL FUNCTIONS
*/
void halMcuWaitMs(uint16 msec);
void  BOARD_INITIALIZATION();
void  SPI_INITIALIZATION();
void  RF_RESET();
void  RF_INITIALIZATION();
void halMcuWaitUs(uint16 usec);

/***********************************************************************************
* Globals 
*/
int result;
uint8 length;
int i;
//  The data sent by the transceiver
uint8 pData[128] = 
{
  0xA5,
  0x5A,
  0xA5,
  0x5A,
  0xA5,
  0x5A,
  0xA5,
  0x5A
};

/***********************************************************************************
* MAIN FUNCTION
*/
void main( void )
{
    //  Initialize led, watchdog, clocks...s 
    BOARD_INITIALIZATION();

    //  Initialize SPi 
    SPI_INITIALIZATION();

    //  Reset the CC2520
    RF_RESET();
    
    //  Initialise the RF registers
    RF_INITIALIZATION();      

    //  Put chip on RX state
    CC2520_INS_STROBE(CC2520_INS_SRXON);  
    halMcuWaitUs(200);
     
    while(1)
    {

#if (NODE_MODE == TRANSMITTER_MODE)
      // Basic transmission
      basicRfSendPacket(0x0001, pData, 6);  // Build and send IEE802.15.4 frame
  
      halMcuWaitMs(100);
      //result = CC2520_MEMRD8(CC2520_EXCFLAG0);
      //result = CC2520_MEMRD8(CC2520_EXCFLAG1);
      //result = CC2520_MEMRD8(CC2520_EXCFLAG2);
      LED3_ON();
      halMcuWaitMs(100);
      
      /*OR
      
      LED3_OFF();
      halMcuWaitMs(1000);

      CC2520_TXBUF(6,pData);                    //  Put on TX buffer
      CC2520_INS_STROBE(CC2520_INS_STXONCCA);      //  Begin transmit
      CC2520_CLEAR_EXC(CC2520_EXC_TX_FRM_DONE); //  Flush buffer
      LED3_ON();
      
      halMcuWaitMs(1000); 
      */
      
#else
      //Basic reception
     
      //  Read the exceptions register to check status and Flag
      //result = CC2520_MEMRD8(CC2520_EXCFLAG0);
      result = CC2520_MEMRD8(CC2520_EXCFLAG1);
      //result = CC2520_MEMRD8(CC2520_EXCFLAG2);
      
      if((result & 0x01) == 0x01) // Check the RX_FRM_DONE flag
      {
          // Clear the RX_FRM_DONE flag    
          result &= ~0x01;
          CC2520_MEMWR8(CC2520_EXCFLAG1,result );
          
          CC2520_RXBUF(1,&length);  // First read RXFIFO length
          CC2520_RXBUF(length,(uint8  *) pData);  // Read RXFIFO
         
          // First byte is the RSSI
          // pData[0]

          halMcuWaitMs(50);
          LED1_ON();                 
          CC2520_INS_STROBE(CC2520_INS_SFLUSHRX); //  Flush RXFIFO
          halMcuWaitUs(200);

      }
      
      halMcuWaitMs(50);
#endif
      
      LED1_OFF();
      LED3_OFF();

      //*/
    }
   
}

/***********************************************************************************
* @fn          halMcuWaitUs
*
* @brief       Busy wait function. Waits the specified number of microseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions. The duration of one cycle depends on MCLK. In this HAL
*              , it is set to 8 MHz, thus 8 cycles per usec.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 usec - number of microseconds delay
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitUs(uint16 usec) // 5 cycles for calling
{
    // The least we can wait is 3 usec:
    // ~1 usec for call, 1 for first compare and 1 for return
    while(usec > 3)       // 2 cycles for compare
    {                // 2 cycles for jump
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        NOP();       // 1 cycles for nop
        usec -= 2;        // 1 cycles for optimized decrement
    }
}                         // 4 cycles for returning


/***********************************************************************************
* @fn          halMcuWaitMs
*
* @brief       Busy wait function. Waits the specified number of milliseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 millisec - number of milliseconds delay
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitMs(uint16 msec)
{
    while(msec-- > 0)
    {
        halMcuWaitUs(1000);
    }
}


/***********************************************************************************
* @fn          SPI_INITIALIZATION
*
* @brief       
*
*              
*
* @param       none
*
* @return      none
*/
void  BOARD_INITIALIZATION()
{
    // Stop watchdog
    WDTCTL = WDTPW + WDTHOLD;
    
    /** Configure XTAL **/
    P7SEL |= BIT0 + BIT1;		      // Activate XT1
    UCSCTL6 &= ~XT1OFF;                       // Set XT1 On
    UCSCTL6 |= XT1DRIVE_2 + XTS + XT2OFF;     // Max drive strength, adjust

    /* led init for wismote */
    LED_DIR();
    LED2_ON();
    LED3_OFF();
    LED1_OFF();
}
/***********************************************************************************
* @fn          RF_RESET
*
* @brief       
*
*              
*
* @param       none
*
* @return      none
*/
    /*
     *  Reset procedure of RF  
    **/  
void  RF_RESET()
{
    P4DIR |= BIT4 | BIT3;                       
    P4OUT &= ~BIT3 ;                                    
    P4OUT |= BIT3 ;                           // Turn the chip on                              
    halMcuWaitUs(2100);
    halMcuWaitUs(CC2520_VREG_MAX_STARTUP_TIME);
    P4OUT &= ~BIT4;                             // Reset the chip            
    halMcuWaitUs(2100);
    P4OUT |= BIT4;
    halMcuWaitUs(2100);
    
    // Enable CS and wait for the chip to be ready
    P3DIR |= BIT0; 
    P3OUT &= ~BIT0;
    while((P3IN & BIT2) != BIT2)
          halMcuWaitMs(1);
    P3OUT |= BIT0;

 }
/***********************************************************************************
* @fn          SPI_INITIALIZATION
*
* @brief       
*
*              
*
* @param       none
*
* @return      none
*/
void  SPI_INITIALIZATION()
{
    /* 
     * SPI Configuration
    **/
    UCB0CTL1 |= UCSWRST;                          // Put state machine in reset
    UCB0CTL1 = UCSSEL0 | UCSSEL1;                 // Select ACLK
    UCB0CTL0 |=  UCCKPH | UCSYNC | UCMSB | UCMST; // 3-pin, 8-bit SPI master, rising edge capture
    
    // 16 bit baud rate register
    UCB0BR0 = 0x00;                         // MSB => 0    
    UCB0BR1 = 0x08;                         // LSB => SMCLK / (UCxxBR0 + UCxxBR1 × 256)
    
    // Set MOSI and SCLK as OUT and MISO as IN ports
    P3SEL |= ( BIT1 | BIT2 | BIT3 );        // Port3 = SPI peripheral
    P3DIR |= ( BIT1 | BIT3 );               //  MOSI and SCLK as Output
    P3DIR &= ~BIT2;                         //  Don't forget to configure MISO as Input
    UCB0IE |= UCTXIE | UCRXIE;              // Enable interrupt for RX and TX SPI buffer
    UCB0CTL1 &= ~UCSWRST;                   // Initialize USCI state machine
    
}

/***********************************************************************************
* @fn          
*
* @brief       
*
*              
*
* @param       nonoe
*
* @return      none
*/
void  RF_INITIALIZATION()
{
    //  Recommended TX settings
    //CC2520_MEMWR8( CC2520_TXPOWER,     0x32);       // Max TX output power => 0xF7
    CC2520_MEMWR8(CC2520_TXCTRL,      0xC1);
    //CC2520_MEMWR8(CC2520_TXCTRL,      0x3F);
    CC2520_MEMWR8(CC2520_TXPOWER,     0xF7);      // Max TX output power
    //CC2520_REGWR8(CC2520_TXPOWER, CC2520_TXPOWER_4_DBM);
    CC2520_MEMWR8(CC2520_CCACTRL0,    0xF8);        // CCA treshold -80dBm

    // Recommended RX settings
    CC2520_MEMWR8(CC2520_MDMCTRL0,    0x84);
    CC2520_MEMWR8(CC2520_MDMCTRL1,    0x14);
    CC2520_MEMWR8(CC2520_RXCTRL,      0x3F); //0x33?
    CC2520_MEMWR8(CC2520_FSCTRL,      0x5A);
    CC2520_MEMWR8(CC2520_FSCAL1,      0x03);
    CC2520_MEMWR8(CC2520_AGCCTRL1,    0x11);
    
    CC2520_MEMWR8(CC2520_FRMFILT0,    0x00);
    
    //  For calibration ??
    CC2520_MEMWR8(CC2520_ADCTEST0,    0x10);
    CC2520_MEMWR8(CC2520_ADCTEST1,    0x0E);
    CC2520_MEMWR8(CC2520_ADCTEST2,    0x03);
  
    CC2520_REGWR8(CC2520_FREQCTRL, CHANNEL_SELECTED);           //  Define channel (between 11 and 25(?to check))
    
#if (NODE_MODE == TRANSMITTER_MODE)
    CC2520_MEMWR16(CC2520_RAM_SHORTADDR, TRANSMITTER_ADDR); //  Define short address of the node
#elif(NODE_MODE == RECEIVER_MODE)
    CC2520_MEMWR16(CC2520_RAM_SHORTADDR, RECEIVER_ADDR); //  Define short address of the node
#endif
    
    CC2520_MEMWR16(CC2520_RAM_PANID, PANID_SELECTED);     //  Define the pan ID of the network 

    //  Set auto CRC on frame
    CC2520_MEMWR8(CC2520_FRMCTRL0,    0x60);
}

