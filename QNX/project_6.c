/* 
Real-Time Operating Systems
Project 6
Chaithanya Gadiyam & Michael Moffitt
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>       /* for sleep() */
#include <stdint.h>       /* for uintptr_t */
#include <hw/inout.h>     /* for in*() and out*() functions */
#include <sys/neutrino.h> /* for ThreadCtl() */
#include <sys/mman.h>     /* for mmap_device_io() */

#define ADCCHANNEL_ADDRESS 0x282
#define ADCGAIN_ADDRESS 0x283
#define ADCLSB_ADDRESS 0X280
#define ADCMSB_ADDRESS 0X281
#define PORT_LENGTH 1
#define CHANNEL_CONFIG 0x44      // configured to channel 4
#define GAIN_CONFIG	0x01			// ADC gain set to 1
#define PORTA_ADDRESS 0x288           
#define CTRL_ADDRESS 0x28B



int main()
{
	printf("program start.......\n");
	int privity_err;
	uintptr_t ADChannel_handle;
	uintptr_t ADGain_handle;
	uintptr_t Data_Handle1;
	uintptr_t Data_Handle2;
	uintptr_t PORTA_HANDLE;
	uintptr_t Ctrl_Handle;
	uint8_t LSB_Data;
	uint8_t MSB_Data;
	uint8_t Status_Val;
	long int ADCval;


	/* Give this thread root permissions to access the hardware */
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1 )
	{
		fprintf( stderr, "can't get root permissions\n" );
		return -1;
	}
	printf("Got access to hardware.......\n");

	ADChannel_handle = mmap_device_io( PORT_LENGTH, ADCCHANNEL_ADDRESS );
	ADGain_handle = mmap_device_io( PORT_LENGTH, ADCGAIN_ADDRESS );
	Data_Handle1 = mmap_device_io( PORT_LENGTH, ADCLSB_ADDRESS );
	Data_Handle2 = mmap_device_io( PORT_LENGTH, ADCMSB_ADDRESS );
	Ctrl_Handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );
	PORTA_HANDLE = mmap_device_io(PORT_LENGTH, PORTA_ADDRESS);

	out8(Ctrl_Handle, 0x00);       //configure portA as output channel
	printf("channel config, gain config set..\n");

	for(;;)
	{

		out8( ADChannel_handle, CHANNEL_CONFIG);
		out8(ADGain_handle, GAIN_CONFIG);
		Status_Val = in8(ADGain_handle);
		if((Status_Val & 0x20) == 0)        // checking wait status bit
		{
			out8(Data_Handle1, 0x80);       //starting the conversion

			while((Status_Val & 0x80) != 0)              //wait for conversion
			{
				Status_Val = in8(ADGain_handle);
			}

			//reading LSB and MSB of 16 bit value
			LSB_Data = in8(Data_Handle1);      
			MSB_Data = in8(Data_Handle2);
			printf("MSB: 0x%02X, LSB: 0x%02X\n",MSB_Data,LSB_Data);

			if((MSB_Data & 0x80) == 0)
			{
				ADCval = (MSB_Data*256) + LSB_Data;
				ADCval = (ADCval * 31)/32767;           //scaling from 15 bit to 5 bit
				if (ADCval == 0x1F)                     //avoiding the magical number
				{
					ADCval = 0x1E;
				}
				printf("5 bit value : %02X\n", ADCval);
				ADCval = ADCval | 0x80;         //setting the clock high
				out8(PORTA_HANDLE, ADCval);
				usleep(20000);                  //wait for 20 ms
				ADCval = ADCval & 0x7F;			//setting the clock low
				out8(PORTA_HANDLE, ADCval);
			}
			else
			{
				printf("Voltage measurement is not within an acceptable range.\n");
				// Magic number representing a problem
				out8(PORTA_HANDLE, 0x1F | 0x80);
				usleep(20000);         //wait for 20 ms
				out8(PORTA_HANDLE, 0x1F & 0x7F);
			}
		}
	}

	return 0;
}
