#include <avr/io.h>

#define INS_RESET 		0b11000000
#define INS_READ		0b00000011
#define INS_READ_RX		0b10010000 //10010nm0
#define INS_WRITE		0b00000010
#define INS_LOAD_TX		0b01000000 //01000abc
#define INS_RTS			0b10000000 //10000nnn
#define INS_READ_STATUS	0b10100000
#define INS_RX_STATUS	0b10110000
#define INS_BIT_MODIFY	0b00000101


#define MODE_NORMAL		0b00000000	
#define MODE_SLEEP		0b00100000			
#define MODE_LOOPBACK	0b01000000
#define MODE_LISTEN		0b01100000
#define MODE_CONFIG		0b10000000
#define MODE_NO_FILTER  0b01100000 //receive filter: no filter (all messages accepted)

#define MASK_MODE		0b11100000
#define MASK_DLC		0b00001111 //mask for data length in TXn control register
#define MASK_SIDL		0b11100000 //mask for bits in id low register
#define MASK_TXREQ0		0b00000100 //transmit request: 1 for unsent message
#define MASK_CANINTF_RX0IF	0b00000001 //data received in receive buffer 0
#define MASK_RECEIVE_FILTER 0b01100000 


#define CANCTRL			0x0F	   //control register for CAN

//#define TXB0CTRL		0x00110000 //control register for TX0
#define TXB0SIDH		0x00110001 //transmit buffer 0 standard identifier high
#define TXB0SIDL		0x00110010 //transmit buffer 0 standard identifier low
#define TXB0DLC			0x00110101 //transmit buffer 0 data length code
#define TXB0D0			0x00110110 //data byte 0 for TX0

#define RXB0CTRL		0x01100000 //Receive buffer 0 control register

//MASK_RECEIVE_FILTER, MODE_NO_FILTER)

void CAN_init(void);
int CAN_send(int id, char* data, int n);
int CAN_test(void);
int CAN_receive(char * data, int rx);



void CAN_reset(void);
void CAN_read(char*, uint8_t, int);
void CAN_read_rx(char*, uint8_t, int);
void CAN_write(char*, uint8_t, int);
void CAN_load_tx(char*, uint8_t, int);
void CAN_rts(uint8_t);
uint8_t CAN_read_status(void);
uint8_t CAN_rx_status(void);
void CAN_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
