#include <stdio.h>
#include "settings.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include <avr/interrupt.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "sram.h"
#include "joystick.h"
#include "butterfly_interface.h"

int main(void) {
	SPI_NoSlave();
	init_UART(BAUD);
	init_SRAM();
	init_joystick();
	SPI_MasterInit();	
	CAN_init();
	printf("Initialization complete\n");


	printf("CAN_test: %d", CAN_test());


	
	
	

	butterfly_menu();

	return 0;	
	
}





