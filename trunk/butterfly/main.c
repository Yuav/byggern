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

char pinb, pine;

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
	// general integer for use in for-loops, etc. 
	int i;
	// general char for use in loops, etc.
	char c;

	// Program initalization
    Initialization();
	
	// Initial state variables
	char *statetext = malloc(TEXTBUFFER_SIZE);
	statetext[0] = '\0';
	statetext[30] = '\0';
	
	// Main loop
	for (;;)
    {
		

		for (i = 0; i < TEXTBUFFER_SIZE-1; i++){
			
			c = SPI_SlaveReceive();
			
			// poll joypad position if a '.' is received
			if(c == '.'){
				check_joypad();
				i--;
			}
			// check if dummy char received
			else if(c == '@'){
				i--;
			}
			//end of string
			else if (c == '\0') {
				statetext[i] = c;
				break;
			}
			//regular char of the string received
			else {
				statetext[i] = c;
			}
		
		}
	
	
		// Puts string on LCD
		LCD_puts(statetext, 1);
		// Turns LCD colon sign off
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

	// Initializing SPI-interface
	SPI_SlaveInit();

	// Initializing joypad
	Joypad_Init();
 
    // Initialize the LCD           
    LCD_Init();                 

	// Interrupt enable
	sei();
}

/*****************************************************************************
*
*   Function name : Joypad_Init
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Initializate the joypad
*
*****************************************************************************/

void Joypad_Init(void){
	// Representations of joypad input pins
 	pinb = (char)0;
	pine = (char)0;

  	// Port PB4, PB6 and PB7:
  	// Internal pullups
 	PORTB = PORTB | 0b11010000;
	// Data direction in
    DDRB = DDRB & 	0b00101111; 

	// Port PE2 and PE3:
	// Internal pullups
	PORTE = PORTE | 0b00001100;
	// Data direction in
    DDRE = DDRE &	0b11110011;

	// Port PE4 as external interrupt to node1 (output from Butterfly)
	DDRE = DDRE |	0b00010000;

	// Enable interrupts
	// Mask ports: PB4, PB6, PB7 enabled
	PCMSK1 = 0 | (1<<PCINT12) | (1<<PCINT14) | (1<<PCINT15);
	// Mask ports: PE2, PE3 enabled, PE4 disabled
	PCMSK0 = (PCMSK0 & 0b11101111) | (1<<PCINT2) | (1<<PCINT3);
	// Enable pin change interrupts
	EIMSK = EIMSK | (1<<PCIE0) | (1<<PCIE1);



}


/*****************************************************************************
*
*   Function name : Joypad_Init
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Check joypad
*
*****************************************************************************/

void check_joypad(void){
//	PORTE = PORTE & !(1<<PE4);

	if (!(pinb & (1<<PB4))){	
		SPI_SlaveTransmit('e');
		return;
	}
	if (!(pinb & (1<<PB6))){	
		SPI_SlaveTransmit('u');
		return;
	}
	if (!(pinb & (1<<PB7))){	
		SPI_SlaveTransmit('d');
		return;
	}
	if (!(pine & (1<<PE2))){	
		SPI_SlaveTransmit('l');
		return;
	}
	if (!(pine & (1<<PE3))){	
		SPI_SlaveTransmit('r');
		return;
	}
	SPI_SlaveTransmit('0');
	return;
}

SIGNAL(SIG_PIN_CHANGE0) {
	pine = (char)PINE;
	PORTE = PORTE | (1<<PE4);
}

SIGNAL(SIG_PIN_CHANGE1) {
	pinb = (char)PINB;
	PORTE = PORTE | (1<<PE4);
}

