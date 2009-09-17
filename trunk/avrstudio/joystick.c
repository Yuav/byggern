#include "settings.h"
#include <avr/io.h>
#include "joystick.h"


#include <util/delay.h>



void init_joystick(){
	// sett opp port B: inn
	DDRB = 0x00; //inputs
	PORTB = 0xFF; //Pull-ups
}

int read_axis(char axis) {
	volatile uint8_t *adc_address = (uint8_t *) 0x1800;
	
	if(axis == 'x'){
		//printf("INTr1: %x\n", PINB);
		//DDRD = 0x00;
		adc_address[255] = 0x04;
		//loop_until_bit_is_clear(PINB, 0);
		_delay_ms(2);
		printf("%c\n", (char)*adc_address);
		return (int)*adc_address;;
	}
	else if(axis == 'y'){
		//DDRD = 0x02;
		adc_address[255] = 0x06;
		//loop_until_bit_is_clear(PINB, 0);
		_delay_ms(2);
		printf("%c\n", *adc_address);
		return (int)*adc_address;
	}
	return -1;
	//evt. korrigering
}

inline uint8_t read_buttons(){
	return PINB & 0x6;
}

/*
adc_address : er en adresse(peker), inneholder verdien 0x1800
*adc_address : er INNHOLDET i adc_address, altså dataen i minneadresse 0x1800

adc_address er definert til å være en adresse som inneholder en uint8_t
*adc_address er dermed definert til å være en uint8_t


*/
