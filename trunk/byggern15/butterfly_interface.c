#include "settings.h"
#include "spi.h"
#include "butterfly_interface.h"
#include "game.h"
#include <util/delay.h>

#define TEXTBUFFER_SIZE 25

char butterfly_poll_joystick(void);

enum menu_state_enum {OVER_THE_TOP = -1, START_SP_POINTS, START_SP_ETERNAL, START_MP_POINTS, START_MP_ETERNAL, BELOW_THE_BOTTOM};

void butterfly_menu(){
	enum menu_state_enum menu_state = OVER_THE_TOP;
	char button = '0',last_button;

	butterfly_print("Velkommen");

	while(1){
		last_button = button;
		
		button = butterfly_poll_joystick();

		if(button != last_button){
			printf("state for: %d\n", menu_state);
			switch(button) {
				case 'l': 					
				case 'r': 							continue;
				case 'u': menu_state--;				break;
				case 'd': menu_state++;				break;
				case 'e': start_game(menu_state);	break;
				case '0': 							continue;
				default:  butterfly_print("error");	break;
			}
			printf("state etter: %d\n", menu_state);
			if (menu_state <= OVER_THE_TOP) menu_state = BELOW_THE_BOTTOM-1;
			if (menu_state >= BELOW_THE_BOTTOM) menu_state = OVER_THE_TOP+1;
			switch(menu_state) {
				case START_SP_POINTS: 	butterfly_print("1 Start enspiller poeng");		break;
				case START_SP_ETERNAL: 	butterfly_print("2 Start enspiller evig");		break;
				case START_MP_POINTS: 	butterfly_print("3 Start flersp poeng");		break;
				case START_MP_ETERNAL: 	butterfly_print("4 Start flersp evig");		break;
				default:			menu_state = OVER_THE_TOP+1;
			}
		}
	}	
}

void butterfly_joystick_test(){

	char button = '0',last_button;

	while(1){
		last_button = button;
		
		button = butterfly_poll_joystick();

		if(button != last_button){
			switch(button) {
				case 'l': butterfly_print("left");		break;
				case 'r': butterfly_print("right");		break;
				case 'u': butterfly_print("up");		break;
				case 'd': butterfly_print("down");		break;
				case 'e': butterfly_print("enter");		break;
				case '0': butterfly_print("center");	break;
				default:  butterfly_print("error");		break;
			}
		}
	}
}

void butterfly_print(char * str){
	int i;
	SPI_SelectSlave(SPI_BUTTERFLY);
	for (i = 0; i < TEXTBUFFER_SIZE-1; i++){ 
		if (str[i] == '\0') break;
		SPI_MasterTransmit(str[i]);
	}
	SPI_MasterTransmit('\0');
	SPI_NoSlave();
}

char butterfly_poll_joystick(){
	char button;	
	SPI_SelectSlave(SPI_BUTTERFLY);
	SPI_MasterTransmit('.');
	_delay_ms(10);
	button = SPI_MasterReceive();
	SPI_NoSlave();
	return button;
}

void start_game(enum menu_state_enum menu_state){
	switch(menu_state) {
		case START_SP_POINTS: 
			butterfly_print("enspiller poeng");
			break;
		case START_SP_ETERNAL: 
			butterfly_print("enspiller evig");
			break;
		case START_MP_POINTS: 	
			butterfly_print("flerspiller poeng");
			break;
		case START_MP_ETERNAL: 	
			butterfly_print("flerspiller evig");	
			break;
		default:			
			return;
	}
	_delay_ms(5000);
}

