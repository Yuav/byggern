#include "can.h"
#include "spi.h"
#include "settings.h"
#include "servo.h"
#include "motor.h"
#include "solenoid.h"

#include "TWI_master.h" 
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

//Initialize the CAN bus

extern unsigned char trig, read_score;
extern int8_t motor_reference, servo;

void CAN_init(void){

	CAN_reset();

	uint8_t data[2];

	//Acceptance mask for RXB0 (all 11 bits counts)
	//dette filteret blokkerer alt?!?
	data[0] = 0b11111111;
	data[1] = 0b11100000;
	CAN_write(data[0], MASK_RXF0);
	CAN_write(data[1], MASK_RXF0+1);


	//RXF0
	//Receive filter 0 hits when id = 0x1F (exactly)	
	data[0] = 0b00000011;
	data[1] = 0b11100000;
	CAN_write(data[0], RXF0);
	CAN_write(data[1], RXF0+1);


	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_NORMAL); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_ID_TYPE, ID_TYPE_STANDARD); // set no filter, set to 01 to accept only standard, 00 to accept accordig to filters
	CAN_bit_modify(CANINTE, 0x01, 0xff); //enable interrupt on receive
	CAN_init_interrupt();
}

// Test send and recieve in loopback mode, verify result from UART
int CAN_test(void){
//	printf("CAN_test started\n");
	int i;
	

	CAN_message message;
	message.id = 0;
	message.length = 8;
	message.data = " ";
	
	return 0;
}

// Send string using CAN bus
int CAN_send(char* str, int id){
	
	unsigned int part, i;
	int done = 0;

	char *messg = "\0\0\0\0\0\0\0";
	for(i = 0; i < 8; i++){
		messg[i] = str[i];
		/*if (messg[i] == '\0') {
			break;
		}*/
	}

	//cli(); // disable interrupts, to protect SPI-communication 

	CAN_bit_modify(TXB0SIDH, 0xFF, (id>>3)); //transmit buffer 0 id high
	CAN_bit_modify(TXB0SIDL, MASK_SIDL, (id<<5));//transmit buffer 0 id low
	//CAN_write((char)messg.length, TXB0DLC);	// data length
	CAN_write((char)8, TXB0DLC);	// data length
	CAN_load_tx(messg, 0); //load transmit buffer from channel 0
	//printf("\n\n%s\n\n", messg.data);
	//_delay_ms(1);
	CAN_rts(0); //request to send

	//wait for send OK ()
	for(i = 0; i < 0xffff; i++){
		if((CAN_read_status() & MASK_TXREQ0) == 0) break;
	}
	//sei(); // enable interrupts again
	if(i == 0xffff) return -1;
	
	return 0;
}

// Recieve string from CAN recieve buffer
int CAN_receive(CAN_message* msg, int rx){
	//FILHIT to check message type
	
	while((CAN_read_status() & MASK_CANINTF_RX0IF+2*rx) == 0); // loop until data received
	CAN_read_rx(msg, rx);

	msg->id = 0x1F; //only id allowed
	
	return 0;

}


void CAN_init_interrupt(){
	//interrupt init
	DDRE = DDRE & 	0b11101111;
	PORTE = PORTE | 0b00010000;
	EICRB = 0;// | (0<<ISC41) | (0<<ISC40); ////////////////fix: ikke or med 0
	EIMSK = EIMSK | (1<<INT4);
}


void sig_interrupt4() {
char str[8];//////


	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

    CAN_receive(&received, 0);

//debug
//	CAN_send(received.data, 0x1F);
char msg[3];/////////

//feiler av en eller annen grunn :S
	if (received.data[0] == (unsigned char)15) { //receive packet starting with 15 (our group number)
	
		switch (received.data[1]) {
			case 'x': //receive joystic x-axis data
				servo = (int8_t) received.data[2];
				break;
			case 'y': //receive joystic y-axis data
				motor_reference = (int8_t) received.data[2];
				break;
			case 'a': //read score
				read_score = 1;
				break;
			case 'b': //joystick button pressed
				trig = 1;
				break;
			default:
				break;
		}
	
	}


}

