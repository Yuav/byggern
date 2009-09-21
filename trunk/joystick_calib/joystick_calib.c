#include <stdio.h>
#include "settings.h"
#include "uart.h"
#include <avr/interrupt.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "sram.h"
#include "joystick.h"

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
float xa1, xa2, xb1, xb2;
xa1 = -128./(xpos_max_hoyre - xpos_0);
xb1 = -xpos_0*xa1;
for(i = 1; i <= xpos_0; i++){
	largevalue = (xa1*i)+xb1;
	if(largevalue < -128) largevalue = -128;
	eeprom_write_byte((uint8_t*)i,(uint8_t)(largevalue));
}

xa2 = 127./(xpos_max_venstre - xpos_0);
xb2 = -xpos_0*xa2;
for(i = xpos_0; i < 256; i++){
	largevalue = (xa2*i)+xb2;
	if(largevalue > 127) largevalue = 127;
	eeprom_write_byte((uint8_t*)i,(uint8_t)(largevalue));
}

float ya1, ya2, yb1, yb2;
ya1 = -128./(ypos_max_bak - ypos_0);
yb1 = -ypos_0*ya1;
for(i = 0; i <= ypos_0; i++){
	largevalue = (i*ya1)+yb1;
	if(largevalue < -128) largevalue = -128;
	eeprom_write_byte((uint8_t*)i+256,(uint8_t)largevalue);
}

ya2 = 127./(ypos_max_fram - ypos_0);
yb2 = -ypos_0*ya2;
for(i = ypos_0; i < 256; i++){
	largevalue = (i*ya2)+yb2;
	if(largevalue > 127) largevalue = 127;
	eeprom_write_byte((uint8_t*)i+256,(uint8_t)largevalue);
}


for(i = 0; i < 512; i++){
	printf("ADC inn: %d  tabell ut: %d\n", i, (int8_t)eeprom_read_byte((uint8_t*)i));
}

return 0;
}
