#include "solenoid.h"
#include "settings.h"
#include <util/delay.h>
#include <avr/io.h>

void solenoid_init(){
	//PF1 out
	PORTF |= 0b00000010;
	DDRF |= 0b00000010;	
}

void trig_solenoid(){
	PORTF &= 0b11111101;
	//_delay_ms(3);
	_delay_ms(15);
	PORTF |= 0b00000010;
}
