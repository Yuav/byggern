#include <avr/io.h>
#include "ir.h"

int ir_blocked = 0;
int score = 0;
int scoring_enabled = 0;

void ir_init(){
	ADMUX = 0b11100000; //internal vref, left adjust, ADC0 single ended
	ADCSRA = 0b11101111;//ACD enable, start conversion, free running, Interrupt flag, interrupt enable, prescaler 128
}

uint8_t get_ir(){
	if (ir_blocked)
		return 0;
	else 
		return 1;
}

//returns score as counted by ir-module
int get_score(){
	return score;
}

void reset_score(){
	score = 0;
}

//method called from interrupt routine whenever an ADC-sample is ready
void adc_interrupt(){
	
	static int i = 0; //average counter
	static int mean = 4; //number of samples for adc
	static int value = 0;
	
	value += ADCH;

	i++;
	if (i >= mean){		

		if ((ir_blocked == 1) && (value > 6*mean)){
			ir_blocked = 0;			
		}
		else if ((ir_blocked == 0) && (value < 2*mean)){
			ir_blocked = 1;
			score++;
		}

		value = 0;
		i = 0;
	}
}
