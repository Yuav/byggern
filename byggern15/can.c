#include "can.h"
#include "spi.h"



#include "settings.h"
#include <util/delay.h>

void CAN_init(void){
	char read[2];
	read[1] = '\0';

	CAN_reset();
	
	CAN_read(read, CANCTRL, 1);
	printf("Read CANCTRL:  ");
	printf("0x%x", (uint8_t)read[0]);
	printf("\n");	
	
	
	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
	CAN_bit_modify(RXB0CTRL, MASK_RECEIVE_FILTER, MODE_NO_FILTER); // set no filter, set to 01 to accept only standard, 00 to accept accordig to filters
	_delay_ms(100);
	CAN_read(read, CANCTRL, 1);
	printf("Read CANCTRL:  ");
	printf("0x%x", (uint8_t)read[0]);
	printf("\n");	

}

int CAN_test(void){
	char received[9];	


	if (CAN_send(0, "hei", 4) != 0){
		return -1;
	}

	CAN_receive(received, 0); //rxbuffer 0

	printf(received);
	printf("\n");

	return 0;
}

int CAN_send(int id, char* data, int n){
	int i;
	CAN_bit_modify(TXB0SIDH, 0xFF, (id<<13)); //transmit buffer 0 id high
	CAN_bit_modify(TXB0SIDL, MASK_SIDL, (id<<5));//transmit buffer 0 id low
	CAN_bit_modify(TXB0DLC, MASK_DLC, n); //data length
	CAN_load_tx(data, 0, n);
	CAN_rts(0);

	//wait for send OK
	for(i = 0; i < 100; i++){
		//printf("0x%x",CAN_read_status());
		if((CAN_read_status() & MASK_TXREQ0) == 0) break;
	}
	if(i == 100) return -1;
	return 0;
}

int CAN_receive(char * data, int rx){
	//FILHIT for å sjekke type beskjed
	int received;
	
	
	while((CAN_read_status() & MASK_CANINTF_RX0IF) == 0);
	//	printf("0x%x",CAN_read_status()); // loop until data received
	CAN_read_rx(data, rx, 8);
	

}






void CAN_reset(void){
	SPI_SelectSlave(SPI_CAN);
	SPI_MasterTransmit(INS_RESET);
	SPI_NoSlave();
}

void CAN_read(char* data, uint8_t address , int data_count){
	int i;
	SPI_SelectSlave(SPI_CAN);	
	SPI_MasterTransmit(INS_READ);	
	SPI_MasterTransmit((char)address);
	for(i = 0; i < data_count; i++){
		data[i] = SPI_MasterReceive();
	}

	SPI_NoSlave();

}

void CAN_read_rx(char* data, uint8_t rx, int data_count){
	int i;
	if (rx>1 || data_count > 8)
		return;
	if(rx == 0) rx = 1; //decode rx0 to word for "read from rxb0", standard frame
	if(rx == 1) rx = 3; //decode rx1 to intruction for "read from rxb1", standard frame
	
	SPI_SelectSlave(SPI_CAN);	
	SPI_MasterTransmit(INS_READ_RX | (rx<<1));
	
	for (i = 0; i < data_count; i++){
		data[i] = SPI_MasterReceive();
	}
	
	SPI_NoSlave();
}

void CAN_write(char* data, uint8_t reg, int data_count){

}
//tx = "modul" (3 output "kanaler")
void CAN_load_tx(char* data, uint8_t tx, int data_count){
	int i;
	if (tx>2 || data_count > 8)
		return;
	uint8_t addr = 0x10*tx + TXB0D0; 
	for(i = 0; i < data_count; i++){
		CAN_bit_modify(addr+i, 0xFF, data[i]); //(address to register, mask = all, data)
	}
}

void CAN_rts(uint8_t tx){
	if (tx == 0) tx = 1;
	else if (tx == 1) tx = 2;
	else if (tx == 2) tx = 4;
	else return;
	
	SPI_SelectSlave(SPI_CAN);
	
	SPI_MasterTransmit(INS_RTS | tx);

	SPI_NoSlave();
}

uint8_t CAN_read_status(void){
	char status;
	SPI_SelectSlave(SPI_CAN);

	SPI_MasterTransmit(INS_READ_STATUS);
	status = SPI_MasterReceive();

	SPI_NoSlave();
	
	return (uint8_t) status;

}

uint8_t CAN_rx_status(void){
return 0;

}
void CAN_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	SPI_SelectSlave(SPI_CAN);

	SPI_MasterTransmit((char)address);
	SPI_MasterTransmit((char)mask);
	SPI_MasterTransmit((char)data);

	SPI_NoSlave();
}
