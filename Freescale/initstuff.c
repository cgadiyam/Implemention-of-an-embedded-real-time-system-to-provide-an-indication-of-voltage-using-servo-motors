#include "initstuff.h"


// Initializes SCI0 for 8N1, 9600 baud, polled I/O
// The value for the baud selection registers is determined
// using the formula:
//
// SCI0 Baud Rate = ( 2 MHz Bus Clock ) / ( 16 * SCI0BD[12:0] )
//--------------------------------------------------------------
void InitializeSerialPort(void)
{
	// Set baud rate to ~9600 (See above formula)
	SCI0BD = 13;					
	
	// 8N1 is default, so we don't have to touch SCI0CR1.
	// Enable the transmitter and receiver.
	SCI0CR2_TE = 1;
	SCI0CR2_RE = 1;
}


// Initializes I/O and timer settings for the demo.
//--------------------------------------------------------------			 
void InitializeTimer(void)
{
	// Set the timer prescaler to %2, since the bus clock is at 2 MHz,
	// and we want the timer running at 1 MHz
	TSCR2_PR0 = 1;
	TSCR2_PR1 = 0;
	TSCR2_PR2 = 0;
		
	// Enable output compare on Channel 1
	TIOS_IOS1 = 1;
	
	// Set up output compare action to toggle Port T, bit 1
	TCTL2_OM1 = 0;
	TCTL2_OL1 = 1;
	
	// Set up timer compare value
	TC1 = TC1_VAL;
	
	// Clear the Output Compare Interrupt Flag (Channel 1) 
	TFLG1 = TFLG1_C1F_MASK;
	
	// Enable the output compare interrupt on Channel 1;
	TIE_C1I = 1;	
	
	//
	// Enable the timer
	// 
	TSCR1_TEN = 1;
	 
	//
	// Enable interrupts via macro provided by hidef.h
	//
	EnableInterrupts;
}


// ISR for the timer. Not really used any more, was used for testing earlier.
#pragma push
#pragma CODE_SEG __SHORT_SEG NON_BANKED
//--------------------------------------------------------------			 
void interrupt 9 OC1_isr( void )
{
//	timer_isr();
	TC1		 +=	TC1_VAL;			
	TFLG1	 =	 TFLG1_C1F_MASK;	
}
#pragma pop


// Blocking putc implementation			 
void TERMIO_PutChar(s8 ch)
{
	// Poll for the last transmit to be complete
	do
	{
		// Nothing	
	} while (SCI0SR1_TC == 0);
	
	// write the data to the output shift register
	SCI0DRL = ch;
}


// Blocking getc implementation			 
u8 GetChar(void)
{ 
	// Poll for data
	do
	{
		// Nothing
	} while(SCI0SR1_RDRF == 0);
	 
	// Fetch and return data from SCI0
	return SCI0DRL;
}                                