#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "servo.h"
#include "can.h"
#include "ir.h"
#include "solenoid.h"

void servo_init(){
	DDRB |= (1<<PB6);

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
