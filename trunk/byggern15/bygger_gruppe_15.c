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
	
	printf("test\n");	

	init_SRAM();
	SPI_MasterInit();	
	CAN_init();

	init_joystick();

/*
	printf("Initialization complete\n");


	printf("CAN_test: %d", CAN_test());


	*/
	
	

	butterfly_menu();

	return 0;	
	
}




SIGNAL(SIG_INTERRUPT0) {
	sig_interrupt0();
}

SIGNAL(SIG_INTERRUPT1) {
	sig_interrupt1();
}

SIGNAL(SIG_OUTPUT_COMPARE0) {
	sig_output_compare0();
}


