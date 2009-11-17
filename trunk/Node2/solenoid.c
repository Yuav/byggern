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
	long long unsigned int i= 0;
	PORTF &= 0b11111101;
	_delay_ms(15);
//	for (i = 0; i < (long long unsigned int)29*15; i++) ;
	
	PORTF |= 0b00000010;
}
