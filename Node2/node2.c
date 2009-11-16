#include <avr/interrupt.h>

#include "settings.h"
#include "can.h"
#include "spi.h"
#include "servo.h"
#include "ir.h"
#include "motor.h"
#include "TWI_master.h"



int main(void) {
	SPI_NoSlave();
	SPI_MasterInit();	
	CAN_init();
	servo_init();
	ir_init();
	solenoid_init();
	//motor_init(); //Henger uten motor tilkoblet

	sei();
	
	//CAN_test();

	while(1){
		//nada
//		asm("nop");
	};

	return 0;	
	
}

SIGNAL(SIG_INTERRUPT4) {
	sig_interrupt4();

}


SIGNAL(SIG_ADC) {	//diode for mål
	adc_interrupt();
}


SIGNAL(SIG_OUTPUT_COMPARE3A) {
	motor_regulator();
}

SIGNAL(SIG_2WIRE_SERIAL){ //////////////////////////////flytt tilbake hvis det virker
	TWI_interrupt();
}
