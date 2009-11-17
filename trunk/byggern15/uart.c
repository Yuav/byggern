#include "settings.h"
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/signal.h>


// Initialize UART interface
int init_UART(unsigned int baud){
	
	//Enable Interrupts. Macro.
	

	unsigned long ubrr0 = baud;
	ubrr0 = (FOSC/(16*ubrr0)) - 1;
	
	unsigned char ucsr0c = 0;

	/* Sette baud rate */
	UBRR0H = (unsigned char)(ubrr0>>8);
	UBRR0L = (unsigned char)ubrr0;
	
	/* Enable receiver og transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	
	/*UCSR0C has settings for frame format*/
	
	/*USBS0: 0 for 1 stop bit, 1 for 2 stop bit*/
	ucsr0c = ucsr0c | (0<<USBS0);

	/*UCSZ01:0 number of databits in payload: 011 for 8 bits. S. 192 in data sheet */
	ucsr0c = ucsr0c | (1<<UCSZ01) | (1<<UCSZ00);	
	
	/*UPM01:0 definerer pairty bit. 00 = parity off, 10 = parity even, 11 parity odd*/
	ucsr0c = ucsr0c | (1<<UPM01) | (0<<UPM00);

	/*URSEL0 must be set to 1 according to data sheet*/
	ucsr0c = ucsr0c | (1<<URSEL0);
		
	UCSR0C = ucsr0c;

	fdevopen(UART_put_char, UART_get_char);

	return 0;

} 

// Send char using UART (can be optimised using interrupt)
int UART_put_char(char c, FILE* dummy){
	if (c == '\n') UART_put_char('\r', dummy);
	loop_until_bit_is_set(UCSR0A, UDRE0); 
	//while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = (unsigned char)c;
	return 0;
}

// Receive char using UART (can be optimised using interrupt)
int UART_get_char(FILE* dummy){
	loop_until_bit_is_set(UCSR0A, RXC0);
	//while ( !(UCSR0A & (1<<RXC0)) );
	char c = UDR0;
	if (c == '\r') c = '\n';
	return (int) UDR0;
}

// Send char using UART with interrupt (disabled for debugging purposes)
/*int UART_put_char(char c, FILE* dummy){

	if (c == '\n') UART_put_char('\r', dummy);
	
	UCSR0B = UCSR0B & ~(1<<UDRIE0);
	
	while (buffer_used >= SEND_BUFFER_LENGTH) {
		UCSR0B = UCSR0B | (1<<UDRIE0);
		UCSR0B = UCSR0B & ~(1<<UDRIE0);
	}


	buffer[buffer_stop] = c;
	buffer_stop ++;
	if (buffer_stop == SEND_BUFFER_LENGTH) buffer_stop = 0;
	buffer_used ++;


	UCSR0B = UCSR0B | (1<<UDRIE0);

	return 0;
}*/

/*SIGNAL(SIG_USART0_DATA)
{
	if (buffer_used != 0) {
		UDR0 = buffer[buffer_start];
		buffer_start ++;
		if (buffer_start >= SEND_BUFFER_LENGTH) buffer_start = 0;
		buffer_used --;
		
	} else {
		UCSR0B = UCSR0B & ~(1<<UDRIE0);
	}
}*/


