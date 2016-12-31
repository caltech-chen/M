//------------------------------------------------------------------------------------
// F04x_Blinky.c
//------------------------------------------------------------------------------------
// Copyright (C) 2007 Silicon Laboratories, Inc.
//
// AUTH: BD
// DATE: 15 MAR 2002
//
// This program flashes the green LED on the C8051F040 target board about five times
// a second using the interrupt handler for Timer3.
// Target: C8051F04x
//
// Tool chain: KEIL Eval 'c'
//

//------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------
#include <c8051f040.h>                    // SFR declarations

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F04x
//-----------------------------------------------------------------------------


//*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------

//#define SYSCLK 3062500                    // approximate SYSCLK frequency in Hz

//sbit  LED = P1^6;                         // green LED: '1' = ON; '0' = OFF


sbit SWITCH = P0^6;

//------------------------------------------------------------------------------------
// Function PROTOTYPES
//------------------------------------------------------------------------------------
void PORT_Init (void);
void UART0_Init();

void UART0_Interrupt();
static char Byte;

//------------------------------------------------------------------------------------
//MAIN Routine
//------------------------------------------------------------------------------------
void main (void) {

char check=0x01;
int i;

WDTCN = 0xde;
WDTCN = 0xad;

SFRPAGE = CONFIG_PAGE; 


   OSCICN = 0x83;                      
   RSTSRC = 0x04;

   OSCXCN = 0x20;                     
   CLKSEL = 0x01;  

                
PORT_Init ();
UART0_Init();

EA = 1;		

SFRPAGE = UART0_PAGE;									// enable global interrupts

SBUF0=0x19;
for ( i=0;i<200;i++);
   while (1) {                            // spin forever
if(TI0==0)
check=(check+1)%255;
SBUF0=check;
for ( i=0;i<10000;i++);
   }
}

//------------------------------------------------------------------------------------
// PORT_Init
//------------------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports
//
void PORT_Init (void)
{
   char SFRPAGE_SAVE=SFRPAGE;

   SFRPAGE=CONFIG_PAGE;
   XBR0=0x04;
   XBR1=0x00;
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups   P0MDOUT |=0x01;
P0MDOUT |= 0x41;
SWITCH=1;

SFRPAGE=SFRPAGE_SAVE;

   //P1MDOUT |= 0x40;                    // enable P1.6 (LED) as push-pull output
}

void UART0_Init()
{
char SFRPAGE_SAVE=SFRPAGE;
 SFRPAGE_SAVE = SFRPAGE;

SFRPAGE = TIMER01_PAGE ;
TL1=0xED ;
TH1=0xED ;


TMOD=0x20;
CKCON=0x01;
ET1=1; 
TCON=0x40;

SFRPAGE = UART0_PAGE;

SSTA0=0x10;
//SCON0=0x50;//8位变bouat
SCON0=0xD0;//9位变bouat
ES0 = 1;
IP |= 0x10;
SFRPAGE=SFRPAGE_SAVE;
}

void UART0_Interrupt (void) interrupt 4
{
   SFRPAGE = UART0_PAGE;

   if (RI0 == 1)
   {
      

      RI0 = 0;
     // Byte = SBUF0;   // Read a character from Hyperterminal
     
      
   }

   if (TI0 == 1)           // Check if transmit flag is set
   {
      TI0 = 0;

     
      
         //SBUF0 = Byte;                   // Transmit to Hyperterminal

        
      
   }
}
                                                                          
//-----------------------------------------------------------------------------
// End Of File
//--------------------------