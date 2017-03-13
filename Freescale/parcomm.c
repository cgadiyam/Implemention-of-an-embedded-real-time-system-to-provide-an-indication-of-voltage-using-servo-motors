#include "parcomm.h"

// Simple support file for reading in a 5-bit value from port A. Blocks on a
// trigger signal on bit 8. Data is valid on the rising edge.  

void parcomm_init(void) 
{
  // Set up both ports as inputs
  DDRA = 0;
  DDRB = 0;  
}

vu8 parcomm_read(void) 
{  
  u8 ret;
  vu16 wait_time;
  // Wait for Bit 7 to rise before reading data
  while (!(PORTA & 0x80)) 
  {
    wait_time++;
  }
  
  // Get low five bits
  ret = PORTA & 0x1F;   
  
  // now wait for falling edge    
  while ((PORTA & 0x80)) 
  {
    wait_time++;
  }
  return ret;
}

vu8 parcomm_read_raw(void) 
{
  return PORTA;
  
}

void wait_button_down(void)
{
  while((parcomm_read_raw() & 0x40)) 
  {
    
  }
  // button is down since value is 0  
}

void wait_button_up(void) 
{
  while(!(parcomm_read_raw() & 0x40)) 
  {
    
  }
  // button is up since value is != 0  
  
}

vu8 is_button_down(void) 
{
  return (!(parcomm_read_raw() & 0x40)); 

}