#include "motor.h"
#include "settings.h"
#include "can.h"
#include "TWI_master.h"
#include <avr/io.h>
#include <stdio.h>

#define P 1.
#define I 0.01
#define ANTI_WIND_UP 127.

int8_t position_ref = 0;

void motor_init(){
	position_ref = 0;

	//initialize timer for motor regulator
	//no output, Fast PWN, top in OCR3A
	TCCR3A = 0b00000011;
	//no noise reduction or capturing, Fast PWN, top in OCR3A, FOSC/1024
	TCCR3B = 0b00011101;

	//Number to count to (here 40 ms, max 40-something)
	long long int count = 40*FOSC/(1000*1024);

	OCR3A = (uint8_t) count;
	
	//Enable interrupt on timer
	ETIMSK |= (1<<OCIE3A);

	//set up TWI
	TWI_Master_Initialise();

	//set up PortA/MJ1

	PORTA = 0x0;
	DDRA = 0xff;

}

void motor_reset(){

}

void motor_set_input(int8_t output){
	static int i = 0;
	i++;
// Har jeg can_send her, henger det bare av og til... :S

	if (i>50){

		char *sendbuf = "\0\0\0\0\0\0\0";
		sprintf(sendbuf, "r:%d", (int)output);
		CAN_send(sendbuf, 0x1F);

	
		i = 0;
	}

	//enable motor
	PORTA |= (1<<5);

	char msg[3];
	msg[0] = (char)0b01010000; //address: MAX520, ADC0, write
	msg[1] = (char)0b00000000; //command: No reset, no power-down, ADC0
	if(output >= 0){
		msg[2] = (char)output*2;
		PORTA &= ~(1<<6);
		}
	else{
		msg[2] = (char)(-output*2)-1;
		PORTA |= (1<<6);
		}
	TWI_Start_Transceiver_With_Data(msg, (unsigned char)3 );
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
	p = ((int)position_ref - motor_read_position())*P;
	
	//I-part
	q += ((int)position_ref - motor_read_position())*I;

	// sette på en can send her gjør at solenoiden henger seg... ved et punkt startet den rett etter 'run' også
	//	char *aa = "\0\0\0\0\0\0\0";
	//	sprintf(aa, "aa:%d", (int)q);
	//	CAN_send(aa, 0x1F);


	if(p+q > ANTI_WIND_UP)
		q = ANTI_WIND_UP-p;
	else if(p+q < -ANTI_WIND_UP-1)
		q = -ANTI_WIND_UP-1-p;

	motor_set_input((int8_t)(p + q));
	
//motor_set_input((int8_t)position_ref);


	// Denne gjorde at UART slutta å printe noe som helst.. random
	//motor_set_input((int)p);


}
