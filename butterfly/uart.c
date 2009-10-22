#include "settings.h"
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/signal.h>

/*char buffer[SEND_BUFFER_LENGTH];
unsigned char buffer_start;
unsigned char buffer_stop;
unsigned char buffer_used;
*/

//Initialize UART
int init_UART(unsigned int baud){
	
	//Enable Interrupts. Macro.
	//sei();

	unsigned long ubrr = baud;
	ubrr = (FOSC/(16*ubrr)) - 1;
	
	unsigned char ucsrc = 0;

//	buffer_start = 0;
//	buffer_stop = 0;
//	buffer_used = 0;

	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	
	/* Enable receiver og transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	
	
	/*UCSRC has settings for frame format*/
	
	/*USBS: 0 for 1 stopbit, 1 for 2 stopbit*/
	ucsrc = ucsrc | (0<<USBS);

	/*UCSZ1:0 number of databits in payload: 011 for 8 bits. S. 192 in datasheet */
	ucsrc = ucsrc | (1<<UCSZ1) | (1<<UCSZ0);	
	
	/*UPM1:0 definerer pairty bit. 00 = parity off, 10 = parity even, 11 parity odd*/
	ucsrc = ucsrc | (1<<UPM1) | (0<<UPM0);

	/*URSEL må alltid skrives til 1*/
	ucsrc = ucsrc | (1<<7);
		
	UCSRC = ucsrc;

	fdevopen(UART_put_char, UART_get_char);

	return 0;

} 

// Send char over UART (blocking, can be optimised with interrupt)
int UART_put_char(char c, FILE* dummy){
	if (c == '\n') UART_put_char('\r', dummy);
	loop_until_bit_is_set(UCSRA, UDRE); 
	//while ( !( UCSRA & (1<<UDRE)) );
	UDR = (unsigned char)c;
	return 0;
}

// Read char over UART (blocking, can be optimised with interrupt)
int UART_get_char(FILE* dummy){
	loop_until_bit_is_set(UCSRA, RXC);
	//while ( !(UCSRA & (1<<RXC)) );
	char c = UDR;
	if (c == '\r') c = '\n';
	return (int) UDR;
}

// Send char over UART with interrupt (disabled for debugging purposes)
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


