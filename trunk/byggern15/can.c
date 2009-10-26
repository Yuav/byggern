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

	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_ID_TYPE, ID_TYPE_STANDARD); // set no filter, set to 01 to accept only standard, 00 to accept accordig to filters
	CAN_bit_modify(BFPCTRL, 0x0f, 0xff);
	CAN_init_interrupt();
}

// Test send and recieve in loopback mode, verify result from UART
int CAN_test(void){
	printf("CAN_test started\n");
	

	

	CAN_message message;
	message.id = 0;
	message.length = 8;
	message.data = "1324567891011121314151617181920";
	

	// Reset will move into configuration mode
	CAN_reset();
	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_ID_TYPE, ID_TYPE_STANDARD);
	CAN_bit_modify(BFPCTRL, 0x0f, 0xff);

	/*// Blanking out recieved buffer (really needed??)
	for (i = 0;(i < 9); i++)
		received[i] = '\0';	*/

	// Sending data in CAN bus
	printf("Sending string: %s", message.data);
	if (CAN_send(message.data, 0) != 0){
		return -1;
	}

	return 0;
}

// Send string using CAN bus
int CAN_send(char* str, int id){
	
	unsigned int part, i;
	int done = 0;

	CAN_message messg;
	messg.id = id;
	for (part = 0; !done; part += 8){
		for(i = 0; i < 8; i++){
			messg.data[i] = str[part+i];
			if (messg.data[i] == '\0') {
				done = 1;
				i++;
				break;
			}
		}

		messg.length = i;
		cli(); // disable interrupts, to protect SPI-communication 

		CAN_bit_modify(TXB0SIDH, 0xFF, (messg.id<<13)); //transmit buffer 0 id high
		CAN_bit_modify(TXB0SIDL, MASK_SIDL, (messg.id<<5));//transmit buffer 0 id low
		CAN_write((char)messg.length, TXB0DLC);	// data length
		CAN_load_tx(&messg, 0); //load transmit buffer from channel 0
		//printf("\n\n%s\n\n", messg.data);
		//_delay_ms(1);
		CAN_rts(0); //request to send

		//WHYY; BUT WHYYYYY?
		//wait for send OK ()
		for(i = 0; i < 0xffff; i++){
			if((CAN_read_status() & MASK_TXREQ0) == 0) break;
		}
		sei(); // enable interrupts again
		if(i == 0xffff) return -1;
	}
	
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
	
	while((CAN_read_status() & MASK_CANINTF_RX0IF) == 0); // loop until data received
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

	printf("Received: ");
    CAN_receive(&received, 0);
	printf("%s\n", received.data);

}
/*
SIGNAL(SIG_INTERRUPT1) {
		
	CAN_message received;
	received.data = "\0\0\0\0\0\0\0\0";

	//CAN_read(received.data, 0b01110110, 8);
	//CAN_read_rx(received.data, 1);
	printf("Received1: ");
    CAN_receive(&received, 1);
	printf("%s\n", received.data);

}*/
