#include <avr/interrupt.h>

#include "settings.h"
#include "can.h"
#include "spi.h"
#include "servo.h"

int main(void) {
	SPI_NoSlave();
	SPI_MasterInit();	
	CAN_init();
	servo_init();
	
	//CAN_test();

	while(1);

	return 0;	
	
}

SIGNAL(SIG_INTERRUPT4) {	
	sig_interrupt4();
}
