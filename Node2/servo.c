#include <avr/io.h>
#include <stdio.h>

#include "servo.h"
#include "can.h"
#include "ir.h"
#include "solenoid.h"

void servo_init(){
	TCCR1A = 0b00100010; //clear output on compare match, fast PWM - count to OCR
	TCCR1B = 0b00011010; //fast PWM - count to OCR, prescaler: divide FCLK by 8 (counter 1 MHz)
	
	OCR1B = 1500+55; //start in mid position (+ tuning)
	ICR1 = 20000+700; //count to 20000 (20 ms) (+ tuning)
}

void set_position(int8_t position) {
	static float value = 0;
	float temp = (630.*position)/128;
	temp += 1500+55;
	if(temp < 935)
		temp = 935;
	else if (temp > 2180)
		temp = 2180;

	value = value*A_SERVO_LP + temp*(1-A_SERVO_LP);
	OCR1B = (int)value;
}


void sig_interrupt4() {

	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

    CAN_receive(&received, 0);

//	CAN_send(received.data, 0x1F);

	if (received.data[0] == (int)15) { //receive packet starting with 15 (our group number)
	
		switch (received.data[1]) {
			case 'x': //receive joystic x-axis data
				set_position((int8_t) received.data[2]);
				break;
			case 'a': //read score
				sprintf(received.data, "%d", get_score());
				CAN_send(received.data, 0x1F);
				break;
			case 'b': //joystick button pressed
				trig_solenoid();
				break;
			default:
				break;
		}
	
	}


}
