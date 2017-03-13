/* 

Real-Time Operating Systems
Project 6
Michael Moffitt & Chaithanya Gadiyam
*/

#include "initstuff.h" 
#include "types.h"
#include "pwm.h"
#include "parcomm.h"

// Servos start at timer 2 (1 used for ISR)
#define INIT_CHANNEL 2

u16 set = 0;

void timer_isr(void)
{        
  // Do nothing, not needed here
}

int main(void)
{
	u8 read; 
								 
	InitializeSerialPort();
	InitializeTimer();
	pwm_init(2);

justbecauseilikeflowcontrol:
  printf("Press button 1 to enable servo control.\r\n");		
  wait_button_down() ;     	
  wait_button_up() ;
	printf("Press the button again to exit.\r\n");	
	for (;;)
	{
	  // Block on 5-bit value from the parallel input
	  read = parcomm_read(); 
	  if (read == 0x1F) 
	  {
	    printf("Voltage measurement out of range\n\r");
	    goto justcheckthebutton;
	  }
	  
	  // Set it to 3/4 its value to scale to motor resolution
	  read = (read >> 2) + (read >> 1); 
	  
	  // Print scaled value to serial port
	  printf("0x%02X\n\r",read);
	  
	  // Commit scaled position to the motor
	  pwm_set(2,read); 
justcheckthebutton:
	  if (is_button_down()) 
	  {     	
       wait_button_up() ;
	     goto justbecauseilikeflowcontrol;
	    
	  }
	}
	return 0;
}
