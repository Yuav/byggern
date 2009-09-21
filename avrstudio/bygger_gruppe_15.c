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
	
	
//	SRAM_test();



	//init_joystick();

//joystick



//ya1 = -128/()


	while(1){
	
		
	//	printf("X-akse: %x Y-akse: %x\n", read_axis('x'), read_axis('y'));
	
	//	read_axis('x');
	//	read_axis('y');
	//	_delay_ms(500);
		

	
	}
	return 0;	
	
}
