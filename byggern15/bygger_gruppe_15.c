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


//joystick



//ya1 = -128/()



	char last_dir;
	printf("Start\n");

//interrupt init
	PORTD = PORTD | 0b00000100;
	DDRB = DDRB & 	0b11111011;
	MCUCR = MCUCR | (1<<ISC01) | (1<<ISC00);
	GICR = GICR | (1<<INT0);
	sei();

	while(1){
	
		
		
		
		//printf("X-akse: %d Y-akse: %d Knapp1: %d knapp2: %d\n", read_axis('x'), read_axis('y'), (read_buttons()&1), ((read_buttons()>>1)&1));
	
		//	read_axis('x');
		//	read_axis('y');	

	
	}
	return 0;	
	
}

SIGNAL(SIG_INTERRUPT0) {
	SPI_MasterTransmit('.');
	_delay_ms(1);
	char dir = SPI_MasterReceive();
	
	switch(dir) {
		case 'l':
			SPI_MasterTransmitString("left");
			printf("left\n");
			break;
		case 'r':
			SPI_MasterTransmitString("right");
			printf("right\n");
			break;
		case 'u':
			SPI_MasterTransmitString("up");
			printf("up\n");
			break;
		case 'd':
			SPI_MasterTransmitString("down");
			printf("down\n");
			break;
		case 'e':
			SPI_MasterTransmitString("enter");
			printf("enter\n");
			break;
		case '0':
			SPI_MasterTransmitString("center12");
			printf("center\n");
			break;
		default:
			SPI_MasterTransmitString("error");
			printf("error: %c\n", dir);
			break;
	}
	
}
