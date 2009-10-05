#include <stdio.h>
#include "settings.h"
#include "uart.h"
#include "spi.h"
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



	char last_dir;
	printf("Start\n");

//interrupt init
	//PORTD = PORTD | 0b00000100;
	DDRD = DDRD & 	0b11111011;
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
	printf("interrupt\n");
	SPI_MasterTransmit('.');
	printf("master transmit . done \n");
	_delay_ms(1);
	char dir = SPI_MasterReceive();
	printf("receive done\n");
	
	switch(dir) {
		case 'l':
			printf("left\n");
			SPI_MasterTransmitString("left");
			
			break;
		case 'r':
			printf("right\n");
			SPI_MasterTransmitString("right");
			
			break;
		case 'u':
			printf("up\n");
			SPI_MasterTransmitString("up");
			
			break;
		case 'd':
			printf("down\n");
			SPI_MasterTransmitString("down");
			
			break;
		case 'e':
			printf("enter\n");
			SPI_MasterTransmitString("enter");
			
			break;
		case '0':
			printf("center\n");
			SPI_MasterTransmitString("center12");
			
			break;
		default:
			printf("error: %c\n", dir);
			SPI_MasterTransmitString("error");
			
			break;
	}
	
}
