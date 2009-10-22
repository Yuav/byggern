#include "settings.h"
#include <avr/io.h>
#include "joystick.h"
#include <avr/eeprom.h>


#include <util/delay.h>


// Initialize the joystick
void init_joystick(){
	// set port B: in
	DDRB = 0x00; //inputs
	PORTB = 0xFF; //Pull-ups
}

// Read joystick position, returns int8
int8_t read_axis(char axis) {
	volatile uint8_t *adc_address = (uint8_t *) 0x1800;
	
	if(axis == 'x'){
		adc_address[0] = 0x04;
		loop_until_bit_is_clear(PINB, 0);
		return (int8_t)eeprom_read_byte((uint8_t*)*adc_address);
	}
	else if(axis == 'y'){
		adc_address[0] = 0x06;
		loop_until_bit_is_clear(PINB, 0);
		return (int8_t)eeprom_read_byte((uint8_t*)(*adc_address+256));
	}
	return -1;
	//evt. korrigering
}


// Read joystick buttons (Can be optimised with interrupt)
inline uint8_t read_buttons(){
	return (PINB & 0x6) >> 1;
}

	//printf("X-akse: %d Y-akse: %d Knapp1: %d knapp2: %d\n", read_axis('x'), read_axis('y'), (read_buttons()&1), ((read_buttons()>>1)&1));
	
		//	read_axis('x');
		//	read_axis('y');	
