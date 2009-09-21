#ifndef _UART_H_
#define _UART_H_

int init_UART(unsigned int);
int UART_put_char(char, FILE*);
int UART_get_char(FILE*);

#define SEND_BUFFER_LENGTH 16 //max 255

#endif //_UART_H_
