#include "settings.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "sram.h"

//Initialize the extended RAM
void init_SRAM(void)
{
	MCUCR =  MCUCR | (1<<SRW10) | (1<<SRE);// | (1<<SRW10));(1<<SRE)
	EMCUCR =  EMCUCR | (1<<SRW00); 
	SFIOR =  SFIOR | (1<<XMM2);
}

//Function to test the extended RAM. Will print out Error!!! in the terminal
//if the content of the RAM is different from what it should be.
void SRAM_test(void)
{
	unsigned char *ram_address = (unsigned char *) 0x1000;
	printf("Starting...\n");

	int i;
	for(i=0;i<0x800;i++)
	{
		ram_address[i] = (char)0x83;
		_delay_ms(2);
	//	printf("skriver RAM no. 0x%x \n", i);
	}

	for(i=0;i<0x800;i++)
	{
		char c = (char)ram_address[i];
		int j = (int)c;
		
		//printf("Adresse: 0x%x ", i);
		//printf("Lest: 0x%x \n", j);
		
		if(ram_address[i] != (char)0x83)
		{
			printf("Error!!! RAM no. 0x%x ",i);
		}
		else{
			//printf("Written OK\n");
		}
		//_delay_ms(2);
	}

	printf("Finished!!!\n");
}
