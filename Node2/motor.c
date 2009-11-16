#include "motor.h"
#include "settings.h"
#include "can.h"
#include "TWI_master.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define P 1.
#define I 0.1
#define ANTI_WIND_UP 127.

int position_ref = 0;

void motor_init(){
	position_ref = 0;

	//initialize timer for motor regulator
	//no output, Fast PWN, top in OCR3A
	TCCR3A = 0b00000011;
	//no noise reduction or capturing, Fast PWN, top in OCR3A, FOSC/1024
	TCCR3B = 0b00011101;

	//set up TWI
	TWI_Master_Initialise();

	//Number to count to (here 40 ms, max 40-something)
	long long int count = 40*FOSC/(1000*1024);

	OCR3A = (uint8_t) count;
	

	//Enable interrupt on timer
	ETIMSK |= (1<<OCIE3A);

	//set up PortA/MJ1

	
	DDRA = 0xff;//output
	PORTA |= (1<<KVAD_OE) | (1<<KVAD_RST); //active low variables set high
	
	//set up PortC/MJ2

	DDRC = 0x0;//input
	PORTC = 0x00;


	//reset kvadraturteller
	PORTA &= ~(1<<KVAD_RST); 
	PORTA |= (1<<KVAD_RST);	

}

void motor_reset(){

}

void motor_set_input(int8_t output){
	static int i = 0;
	i++;
	// can_send her, henger seg av og til

	/*if (i>50){

		char *sendbuf = "\0\0\0\0\0\0\0";
		sprintf(sendbuf, "r:%d", (int)output);
		CAN_send(sendbuf, 0x1F);

	
		i = 0;
	}*/

	//enable motor
	PORTA |= (1<<MOTOR_EN);

	char msg[3];
	msg[0] = (char)0b01010000; //address: MAX520, ADC0, write
	msg[1] = (char)0b00000000; //command: No reset, no power-down, ADC0
	if(output >= 0){
		msg[2] = (char)output*2;
		PORTA &= ~(1<<MOTOR_DIR);
		}
	else{
		msg[2] = (char)(-output*2)-1;
		PORTA |= (1<<MOTOR_DIR);
		}
	
	/*int j, loop = 100;
	for (j = 0; j < loop; j++) {
		if(!TWI_Transceiver_Busy())
			break;
		else if(j == loop)
			TWI_Master_Initialise();//force done
	}

	while(TWI_Transceiver_Busy()); /////returnerer ikke; TWI interrupt er altså enablet. sending av siste feilet?*/
	TWI_Start_Transceiver_With_Data(msg, (unsigned char)3 );
}

int8_t motor_get_position(){
	static long long int position_12_bits = 0;
	int value_read;

	PORTA &= ~(1<<KVAD_OE);
	PORTA &= ~(1<<KVAD_SEL); //!OE and SEL set low
	
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	value_read = ((int)PINC<<8); //read highest byte

	PORTA |= (1<<KVAD_SEL); //SEL high

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	//sign-extend value_read with the MSB of the 12-bit value
	if ((value_read>>11) && 1){
		value_read |= 0xF000; //negative value, fill with 1's
		value_read -= (unsigned char)PINC; //read and add lowest byte
	}else{
		value_read &= 0x0FFF; //positive value, fill with 0's
		value_read += (unsigned char)PINC; //read and add lowest byte
	}	

	//reset
	PORTA &= ~(1<<KVAD_RST); 
	PORTA |= (1<<KVAD_RST);		

	PORTA |= (1<<KVAD_OE); //output disable
	
	//return (int)((position_12_bits += value_read)>>8);
	return 0;		
}

void motor_set_reference(int8_t ref){
	position_ref = (int)ref; ////sign-extension?
}

void motor_regulator() {
	float p = 0;
	static float q = 0;

	//P-part:
	p = (position_ref - motor_get_position())*P;
	
	//I-part
	q += (position_ref - motor_get_position())*I;
	//	CAN_send(aa, 0x1F);


	if(p+q > ANTI_WIND_UP)
		q = ANTI_WIND_UP-p;
	else if(p+q < -ANTI_WIND_UP-1)
		q = -ANTI_WIND_UP-1-p;

	motor_set_input((int8_t)(p + q));


}
/*
void motor_DAC_send_loop(){
	while(1){
		cli();
		TWI_Start_Transceiver_With_Data(DAC_data, (unsigned char)3 );
		sei();
	}
		
}
*/
