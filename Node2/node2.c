#include <avr/interrupt.h>

#include "settings.h"
#include "can.h"
#include "spi.h"
#include "servo.h"
#include "ir.h"
#include "motor.h"
#include "TWI_master.h"
//#include <stdio.h>
//#include <util/delay.h>

unsigned char trig = 0, read_score = 0; //bool for trigging solenoid
int8_t motor_reference, servo; //motor reference to be set

int main(void) {
	char *send_buf = "\0\0\0\0\0\0\0"
	
	cli();
	SPI_NoSlave();
	SPI_MasterInit();	
	CAN_init();

	//CAN_send("N2 - OK", 0x1F);	

	servo_init();
	ir_init();
	solenoid_init();
	motor_init(); //Henger uten motor tilkoblet

	

	sei();
	
	//CAN_test();
	int8_t pos = 0;
	while(1){
		motor_set_reference(motor_reference); //set motor position
		set_position(servo); //set servo position
		if (trig) {	//trig solenoid
			trig_solenoid();
			trig = 0;
		}
		if (read_score) { //read score and send over CAN	
			//sprintf(send_buf, "%c%c%d", (unsigned char) 15, 's', get_score());
			CAN_send(send_buf, 0x1F);
			read_score = 0;
		}
	}

	return 0;	
	
}

SIGNAL(SIG_INTERRUPT4) {
	sig_interrupt4(); //CAN
}


SIGNAL(SIG_ADC) {	//diode for mål
	//adc_interrupt();
}


SIGNAL(SIG_OUTPUT_COMPARE3A) {
	motor_regulator();
}

SIGNAL(SIG_2WIRE_SERIAL){ //////////////////////////////flytt tilbake hvis det virker
	
	TWI_interrupt();
	
}
