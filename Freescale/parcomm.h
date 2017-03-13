#ifndef PARCOMM_H
#define PARCOMM_H
#include "types.h"   
#include "mc9s12dt256.h"

// Simple support file for reading in a 5-bit value from port A. Blocks on a
// trigger signal on bit 8. Data is valid on the rising edge. 
// Pushbutton is connected to bit 6, 0x40

// Connection made on Port A:

// 0 - d0 (lsb)
// 1 - d1
// 2 - d2
// 3 - d3
// 4 - d4 (msb)
// 5 - n/c
// 6 - Button
// 7 - Capture ready signal (active high)
 
void parcomm_init(void);
vu8 parcomm_read_raw(void);
vu8 parcomm_read(void);

void wait_button_down(void);
void wait_button_up(void);
vu8 is_button_down(void);

#endif