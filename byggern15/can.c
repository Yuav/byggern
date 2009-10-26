#include "can.h"
#include "spi.h"
#include "settings.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

//Initialize the CAN bus
void CAN_init(void){

	CAN_reset();

	uint8_t data[2];

	//Acceptance mask for RXB0 (all 11 bits counts)
	data[0] = 0b11111111;
	data[1] = 0b11100000;
	CAN_write(data[0], MASK_RXF0);
	CAN_write(data[1], MASK_RXF0+1);


	//RXF0
	//Receive filter 0 hits when id = 0x1F (exactly)	
	data[0] = 0b00111111;
	data[1] = 0b11100000;
	CAN_write(data[0], RXF0);
	CAN_write(data[1], RXF0+1);

	//RXF1
	//Receive filter 1 hits when id = 1E (exactly)
	data[0] = 0b00111111;
	data[1] = 0b11000000; 
	CAN_write(data[0], RXF1);
	CAN_write(data[1], RXF1+1);

/*

	//Acceptance mask for RXB1
	data[0] = 0b11111111;
	data[1] = 0b11100000;
	CAN_write(data[0], MASK_RXF1);
	CAN_write(data[1], MASK_RXF1+1);

	//RXF2
	//Receive filter 2 hits when id = 0 (exactly)
	data[0] = 0b00000000;
	data[1] = 0b00000000;
	CAN_write(data[0], RXF2);
	CAN_write(data[1], RXF2+1);

	//RXF3
	//Receive filter 3 hits when id = 1 (exactly)
	data[0] = 0b00000000;
	data[1] = 0b00100000;
	CAN_write(data[0], RXF3);
	CAN_write(data[1], RXF3+1);

	//RXF4
	//Receive filter 4 hits when id = 2 (exactly)
	data[0] = 0b00000000;
	data[1] = 0b01000000;
	CAN_write(data[0], RXF4);
	CAN_write(data[1], RXF4+1);

	//RXF5
	//Receive filter 5 hits when id = 3 (exactly)
	data[0] = 0b00000000;
	data[1] = 0b01100000;
	CAN_write(data[0], RXF5);
	CAN_write(data[1], RXF5+1);
*/

	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_ID_TYPE, ID_TYPE_STANDARD); // set no filter, set to 01 to accept only standard, 00 to accept accordig to filters
	CAN_bit_modify(BFPCTRL, 0x0f, 0xff);
	CAN_init_interrupt();
}

// Test send and recieve in loopback mode, verify result from UART
int CAN_test(void){
	printf("CAN_test started\n");
	int i;
	

	CAN_message message;
	message.id = 0;
	message.length = 8;
	message.data = " ";
	

	// Reset will move into configuration mode
/*	CAN_reset();
	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_ID_TYPE, ID_TYPE_STANDARD);
	CAN_bit_modify(BFPCTRL, 0x0f, 0xff);
*/


	for(i = 0; i < 10; i++){
		switch (i) {
			case 0:
				message.data = "7";
				message.id = 31;
				break;
			case 1:
				message.data = "1";
				message.id = 1;
				break;
			case 2:
				message.data = "2";
				message.id = 2;
				break;
			case 3:
				message.data = "3";
				message.id = 3;
				break;
			case 4:
				message.data = "4";
				message.id = 4;
				break;
			case 5:
				message.data = "5";
				message.id = 5;
				break;
			case 6:
				message.data = "6";
				message.id = 6;
				break;
			case 7:
				message.data = "7";
				message.id = 7;
				break;
			case 8:
				message.data = "8";
				message.id = 8;
				break;
			case 9:
				message.data = "9";
				message.id = 9;
				break;
		}

		/*// Blanking out recieved buffer (really needed??)
		for (i = 0;(i < 9); i++)
			received[i] = '\0';	*/

		// Sending data in CAN bus
		printf("Sending string: %s", message.data);
		if (CAN_send(message.data, message.id) != 0){
			return -1;
		}
	}
	return 0;
}

// Send string using CAN bus
int CAN_send(char* str, int id){
	
	unsigned int part, i;
	int done = 0;

	char *messg = "\0\0\0\0\0\0\0";
	for(i = 0; i < 8; i++){
		messg[i] = str[i];
		if (messg[i] == '\0') {
			break;
		}
	}

	cli(); // disable interrupts, to protect SPI-communication 

	CAN_bit_modify(TXB0SIDH, 0xFF, (id<<13)); //transmit buffer 0 id high
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
	sei(); // enable interrupts again
	if(i == 0xffff) return -1;
	
	
/*
	for (i = 0; 1; i++){
		messg.data[i%8] = str[i];
		
		if(i%8 == 7){
			
			CAN_bit_modify(TXB0SIDH, 0xFF, (messg.id<<13)); //transmit buffer 0 id high
			CAN_bit_modify(TXB0SIDL, MASK_SIDL, (messg.id<<5));//transmit buffer 0 id low
			CAN_write(messg.length, TXB0DLC, 1);	// data length
			CAN_load_tx(&messg, 0); //load transmit buffer from channel 0
			CAN_rts(0); //request to send
	

			//wait for send OK ()
			for(i = 0; i < 0xffff; i++){
				if((CAN_read_status() & MASK_TXREQ0) == 0) break;
			}
			if(i == 0xffff) return -1;
		}

		if(str[i] == '\0') break;
*/
	return 0;
}

// Recieve string from CAN recieve buffer
int CAN_receive(CAN_message* msg, int rx){
	//FILHIT to check message type
	
	while((CAN_read_status() & MASK_CANINTF_RX0IF+2*rx) == 0); // loop until data received
	CAN_read_rx(msg, rx);
	
	return 0;

}





/*void CAN_init_interrupt(){
//interrupt init
	PORTD = PORTD | 0b00000100;
	DDRD = DDRD & 	0b11111011;
	MCUCR = MCUCR | (0<<ISC01) | (0<<ISC00);
	GICR = GICR | (1<<INT0);
	sei();
}*/


void CAN_init_interrupt(){
//interrupt init
	PORTD = PORTD | 0b00001100;
	DDRD = DDRD & 	0b11110011;
	MCUCR = MCUCR | (0<<ISC01) | (0<<ISC00) | (0<<ISC11) | (0<<ISC10);
	GICR = GICR | (1<<INT0) | (1<<INT1);
	sei();
}


SIGNAL(SIG_INTERRUPT0) {
		
	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

	printf("Received0: ");
    CAN_receive(&received, 0);
	printf("%s\n", received.data);

}

SIGNAL(SIG_INTERRUPT1) {
		
	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

	printf("Received1: ");
    CAN_receive(&received, 1);
	printf("%s\n", received.data);
	printf("lala\n");

}
