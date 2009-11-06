#include <avr/io.h>

#include "servo.h"
#include "can.h"


void servo_init(){
	TCCR1A = 0b00100010; //clear output on compare match, fast PWM - count to OCR
	TCCR1B = 0b00011010; //fast PWM - count to OCR, prescaler: divide FCLK by 8 (counter 1 MHz)
	
	OCR1B = 1500+55; //start in mid position
	ICR1 = 20000+700; //count to 20000 (20 ms)
}

void set_position(int8_t position) {
	long int temp = ((long int)630*position)/128;
	temp += 1500+55;
	if(temp < 935)
		temp = 935;
	else if (temp > 2180)
		temp = 2180;

	OCR1B = (int)temp;
}


void sig_interrupt4() {
		

	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

    CAN_receive(&received, 0);
	if (received.data[0] == (int)15) {
	
		switch (received.data[1]) {
			case 'x':
				set_position((int8_t) received.data[2]);
				break;
			default:
				break;
		}
	
	}


}
