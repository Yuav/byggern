#include <stdio.h>
#include "settings.h"
#include "uart.h"
#include <avr/interrupt.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "sram.h"
#include "joystick.h"

#define DEADSPACE_INNER_LOW 4
#define DEADSPACE_INNER_HIGH 10
#define DEADSPACE_OUTER_LOW 5
#define DEADSPACE_OUTER_HIGH 12


int main(void) {
	

	init_UART(BAUD);



//		posisjon	 	 	ADC input
uint8_t	xpos_0 				= 114;
uint8_t	xpos_max_hoyre 		= 76;
uint8_t	xpos_max_venstre 	= 227;

//		posisjon	 	 	ADC input
uint8_t	ypos_0 				= 113;
uint8_t	ypos_max_fram 		= 231;
uint8_t	ypos_max_bak	 	= 76;

int i, largevalue;

// x-retning
float xa1, xa2, xb1, xb2;
//mindre enn null (mot venstre)
xa1 = 127./((xpos_max_hoyre+DEADSPACE_OUTER_LOW) - (xpos_0-DEADSPACE_OUTER_LOW));
xb1 = (-xpos_0+DEADSPACE_OUTER_LOW)*xa1;

for(i = 1; i <= xpos_0; i++){
	largevalue = (xa1*i)+xb1;

	if(largevalue > 127) largevalue = 127;
	if(largevalue < 0) largevalue = 0;

	eeprom_write_byte((uint8_t*)i,(uint8_t)(largevalue));
}

//større enn null (mot høyre)
xa2 = -128./((xpos_max_venstre-DEADSPACE_OUTER_HIGH) - (xpos_0+DEADSPACE_INNER_HIGH));
xb2 = (-xpos_0-DEADSPACE_INNER_HIGH)*xa2;


for(i = xpos_0; i < 256; i++){
	largevalue = (xa2*i)+xb2;
	if(largevalue < -128) largevalue = -128;
	if(largevalue > 0) largevalue = 0;
	eeprom_write_byte((uint8_t*)i,(uint8_t)(largevalue));
}

// y-retning
float ya1, ya2, yb1, yb2;
ya1 = -128./((ypos_max_bak+DEADSPACE_OUTER_LOW) - (ypos_0-DEADSPACE_INNER_LOW));
yb1 = (-ypos_0+DEADSPACE_INNER_LOW)*ya1;
for(i = 0; i <= ypos_0; i++){
	largevalue = (i*ya1)+yb1;
	if(largevalue < -128) largevalue = -128;
	if(largevalue > 0) largevalue = 0;
	eeprom_write_byte((uint8_t*)i+256,(uint8_t)largevalue);
}


//Oppover
ya2 = 127./((ypos_max_fram-DEADSPACE_OUTER_HIGH) - (ypos_0+DEADSPACE_INNER_HIGH));
yb2 = (-ypos_0-DEADSPACE_INNER_HIGH)*ya2;

for(i = ypos_0; i < 256; i++){
	largevalue = (i*ya2)+yb2;
	if(largevalue > 127) largevalue = 127;
	if(largevalue < 0) largevalue = 0;
	eeprom_write_byte((uint8_t*)i+256,(uint8_t)largevalue);
}


for(i = 0; i < 256; i++){
	printf("ADCx inn: %d  tabell ut: %d\n", i, (int8_t)eeprom_read_byte((uint8_t*)i));
}
for(i = 0; i < 256; i++){
	printf("ADCy inn: %d  tabell ut: %d\n", i, (int8_t)eeprom_read_byte((uint8_t*)(i+256)));
}

return 0;
}
