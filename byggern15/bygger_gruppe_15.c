#include <stdio.h>
#include "settings.h"
#include "uart.h"
#include <avr/interrupt.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "sram.h"
#include "joystick.h"

int main(void) {
	

	init_UART(BAUD);

	init_SRAM();

	init_joystick();
	
	SPI_MasterInit();	
//	SRAM_test();



	

//joystick



//ya1 = -128/()


	while(1){
	
	
	SPI_MasterTransmit('5');
		
	//printf("X-akse: %d Y-akse: %d Knapp1: %d knapp2: %d\n", read_axis('x'), read_axis('y'), (read_buttons()&1), ((read_buttons()>>1)&1));
	
	//	read_axis('x');
	//	read_axis('y');
				

	
	}
	return 0;	
	
}
