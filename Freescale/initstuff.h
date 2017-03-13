#ifndef INITSTUFF_H
#define INITSTUFF_H

// Initialization code for Project 6

// system includes
#include <hidef.h>			/* common defines and macros */
#include <stdio.h>			/* Standard I/O Library */

// project includes
#include "derivative.h" /* derivative-specific definitions */   
#include "types.h"

// Definitions


// Change this value to change the frequency of the output compare signal.
// The value is in Hz.

// Macro definitions for determining the TC1 value for the desired frequency
// in Hz (OC_FREQ_HZ). The formula is:
//
// TC1_VAL = ((Bus Clock Frequency / Prescaler value) / 2) / Desired Freq in Hz
//
// Where:
//				Bus Clock Frequency		 = 2 MHz
//				Prescaler Value				 = 2 (Effectively giving us a 1 MHz timer)
//				2 --> Since we want to toggle the output at half of the period
//				Desired Frequency in Hz = The value you put in OC_FREQ_HZ
//                              
#define OC_FREQ_HZ		((u16)10)
#define BUS_CLK_FREQ	((u32) 2000000)	 
#define PRESCALE			((u16)	2)				 
#define TC1_VAL			 ((u16)	(((BUS_CLK_FREQ / PRESCALE) / 2) / OC_FREQ_HZ))
     
void timer_isr(void);
void InitializeSerialPort(void);
void InitializeTimer(void);
void TERMIO_PutChar(s8 ch);
u8 GetChar(void);

#endif