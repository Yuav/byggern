#include "settings.h"
#include <avr/io.h>
#include "joystick.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#include <util/delay.h>


// Initialize the joystick
void init_joystick(){
	// set port B: in
	DDRB &= 0b11111110; //inputs
	PORTB |= 0b00000000; //Pull-ups (not on adc ready)

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
	
	switch (axis){
		case 'x':
			adc_address[0] = 0x04;
			loop_until_bit_is_clear(PINB, 0);
			return (int8_t)eeprom_read_byte((uint8_t*)*adc_address);
			
		case 'y':
			adc_address[0] = 0x05;
			loop_until_bit_is_clear(PINB, 0);
			return (int8_t)eeprom_read_byte((uint8_t*)((*adc_address)+256));
		default:
			return 0;
	}

	return -1;
	//evt. korrigering
}

	//printf("X-akse: %d Y-akse: %d Knapp1: %d knapp2: %d\n", read_axis('x'), read_axis('y'), (read_buttons()&1), ((read_buttons()>>1)&1));
	
		//	read_axis('x');
		//	read_axis('y');	

void sig_output_compare0() {

	static int temp;
	temp+=1;

	//check joystick
	char *str = "\0\0\0\0\0\0\0"; 

	str[0] = (char)15; //group 15

//Ved å sette x = y her henger det ikke uten printf...
	str[1] = 'x'; //x axis ///////////////////////////

	str[2] = (char)read_axis('x'); //data

//Printf her henger på SPI, og kretsen ser ut til å resette en del.. men kun ved bevegelse på joystick!

//Følgende printf resetter kretsen etter NØYAKTIG 11 printf
//	printf("X-Akse: %d",str[2]);

	CAN_send(str, 0x1F);

	
	if (!(temp%20)){
		printf("x: %d\n", str[2]);
	}

	//y-axis


	/////y-axis: henger på loop_until_bit_is_clear etter noen iterasjoner (350-400)
	str[1] = 'y'; //y axis

	str[2] = (char)read_axis('y'); //data

	CAN_send(str, 0x1F);
	
	
	if (!(temp%20)){
		printf("y: %d\n", str[2]);
	}
	
	
	
	




	//read score

//ved å kommentere ut det under henger den etter NØYAKTIG 10 (ikke noe loop) printf

	
	
}

void int_joystick_button(){
	printf("Joystick knapp\n");
	char *str = "\0\0\0\0\0\0\0";
	str[0] = (char)15; //group 15
	str[1] = 'b'; //button
	CAN_send(str, 0x1F);
	
}
