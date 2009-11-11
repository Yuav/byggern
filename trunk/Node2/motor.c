#include "motor.h"
#include "settings.h"
#include "can.h"
#include <avr/io.h>
#include <stdio.h>

#define P 1.
#define I 1.
#define ANTI_WIND_UP 100.

int8_t position_ref = 0;

void motor_init(){
	position_ref = 0;

	//initialize timer for joystick polling
	//no output, Fast PWN, top in OCR3A
	TCCR3A = 0b00000011;
	//no noise reduction or capturing, Fast PWN, top in OCR3A, FOSC/1024
	TCCR3B = 0b00011101;

	//Number to count to (here 40 ms, max 40-something)
	long long int count = 40*FOSC/(1000*1024);

	OCR3A = (uint8_t) count;
	
	//Enable interrupt on CTC
	ETIMSK |= (1<<OCIE3A); 
}

void motor_reset(){

}

void motor_set_input(int8_t pos){
	static int i = 0;
	i++;
// Har jeg can_send her, henger det bare av og til... :S

	if (i>50){

		char *sendbuf = "\0\0\0\0\0\0\0";
		sprintf(sendbuf, "r:%d", (int)pos);
		CAN_send(sendbuf, 0x1F);

	
		i = 0;
	}
}

int8_t motor_read_position(){
	return 0;
}

void motor_set_reference(int8_t ref){
	position_ref = ref;
}

void motor_regulator() {
	float p = 0;
	static float q = 0;

	//P-part:
	p = ((int)position_ref - (int)motor_read_position)*P;
	
	//I-part
	q += ((int)position_ref - (int)motor_read_position)*I/100;

	// sette på en can send her gjør at solenoiden henger seg... ved et punkt startet den rett etter 'run' også
	//	char *aa = "\0\0\0\0\0\0\0";
	//	sprintf(aa, "aa:%d", (int)q);
	//	CAN_send(aa, 0x1F);


	if(q > ANTI_WIND_UP)
		q = ANTI_WIND_UP;
	else if(q < -ANTI_WIND_UP)
		q = -ANTI_WIND_UP;

	motor_set_input((int8_t)(p + q));
	
//motor_set_input((int8_t)position_ref);


	// Denne gjorde at UART slutta å printe noe som helst.. random
	//motor_set_input((int)p);


}
