
//  mt - This code has only been used for debugging 
//  It may not be up-to-date. Please refert to the
//  Current LCD-driver code can be found in the
//  application's gcc-port.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "lcd_functions.h"
#include "lcd_driver.h"
#include "uart.h"
#include "spi.h"


#define pLCDREG_test (*(char *)(0xEC))

// extern unsigned int LCD_character_table[] PROGMEM;

/*****************************************************************************
*
*   Function name : main
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Contains the main loop of the program
*
*****************************************************************************/
int main(void)
{    
	int i;
	char c;
	SPI_SlaveInit();
	
	 
	
	
//	while(1) printf("Hallo\n");
	// mt static char __flash *statetext;
	//PGM_P statetext;
	
	// Initial state variables
	char *statetext = malloc(TEXTBUFFER_SIZE);
	statetext[0] = '\0';
	statetext[30] = '\0';

	// Program initalization
    Initialization();
    sei(); // mt __enable_interrupt();
	
	for (;;)            // Main loop
    {
		

		for (i = 0; i < TEXTBUFFER_SIZE-1; i++){
			
			c = SPI_SlaveReceive();
			
			//hvis tegn = poll joypad: poll joypad og send tilbake
			if(c == '.'){
				check_joypad();
				i--;
			}
			//hvis tegn = \0; putt i string, break loop
			else if (c == '\0') {
				statetext[i] = c;
				break;
			}
			//else putt tegn i string
			else {
				statetext[i] = c;
			}
		
		}
	
	
	
		LCD_puts(statetext, 1);
		LCD_Colon(0);  
	
    } //End Main loop
	
	return 0;
}

/*****************************************************************************
*
*   Function name : Initialization
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Initializate the different modules
*
*****************************************************************************/
void Initialization(void)
{
	/*
	CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable

    // set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
    CLKPR = (1<<CLKPS1) | (1<<CLKPS0);

    // Disable Analog Comparator (power save)
    ACSR = (1<<ACD);

    // Disable Digital input on PF0-2 (power save)
    DIDR1 = (7<<ADC0D);
	*/

    // mt PORTB = (15<<PORTB0);       // Enable pullup on 
//	PORTB = (0x0F<<PB0);       // Enable pullup on 
    // mt PORTE = (15<<PORTE4);
//	PORTE = (15<<PE4);

  
  PORTB = PORTB | 0b11010000; 
    DDRB = DDRB & 0b00101111; 
	PORTE = PORTE | 0b00001100; 
      DDRE = DDRE & 0b11110011; 
//	sbi(DDRB, 5);               // set OC1A as output
  //  sbi(PORTB, 5);              // set OC1A high
	
                
    LCD_Init();                 // initialize the LCD
}


void check_joypad(void){
	return;
}


