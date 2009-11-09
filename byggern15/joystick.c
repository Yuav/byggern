#include "settings.h"
#include <avr/io.h>
#include "joystick.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include <util/delay.h>


// Initialize the joystick
void init_joystick(){
	// set port B: in
//	DDRB = DDRB & 0b11111001; //inputs
//	PORTB = PORTB | 0b00000110; //Pull-ups

	DDRE = DDRE & 0b11111110; //inputs
	PORTE = PORTE | 0b00000001; //Pull-ups

	EMCUCR &= 0b11111110;// trig on falling edge
	GICR |= (1<<INT2); //enable interrupts for this one

	//initialize timer for joystick polling
	//CTC mode, no output, 1024 prescaler
	TCCR0 = 0b00001101;

	//Number to count to (here 40 ms, max 40-something)
	long long int count = 10*FOSC/(1000*1024);

	OCR0 = (uint8_t) count;
	
	//Enable interrupt on CTC
	TIMSK = TIMSK | (1<<OCIE0); 

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


void sig_output_compare0() {
	//check joystick
	char *str = "\0\0\0\0\0\0\0"; 
/*
	str[0] = (char)15; //group 15

	str[1] = 'x'; //x axis

	str[2] = (char)read_axis('x'); //data*/

	static int temp;
	temp+=1;

if (!(temp%20)){
	str[0] = (char)15; //group 15

	str[1] = 'a'; //x axis

	CAN_send(str, 0x1F);
	}
}

void int_joystick_button(){
	
	char *str = "\0\0\0\0\0\0\0";

	str[0] = (char)15; //group 15

	str[1] = 'b'; //x axis

	CAN_send(str, 0x1F);
	
	/*while (!(PINE && 1));
	//loop a while
	cli();
	long long int i;
	for (i = 0; i < 160000;i++){
		//nada
	}
	//reset interrupt
	GIFR |= (1<<INTF2);
	sei();*/
}
