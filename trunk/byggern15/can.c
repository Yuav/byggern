#include "can.h"
#include "spi.h"

void CAN_init(void){
	CAN_reset();
	CAN_bit_modify(CANCTRL, MASK_MODE, MODE_LOOPBACK); //set loopback mode
}

int CAN_test(void){
	if (CAN_send(0, "hei", 3) == 0){
		return 0;
	} else return -1;

}

int CAN_send(int id, char* data, int n){
	int i;
	CAN_bit_modify(TXB0SIDH, 0xFF, (id<<13));
	CAN_bit_modify(TXB0SIDL, MASK_SIDL, (id<<5));
	CAN_bit_modify(TXB0DLC, MASK_DLC, n);
	CAN_load_tx(data, 0, n);
	CAN_rts(0);

	//wait for send OK
	for(i = 0; i < 100; i++){
		if(CAN_read_status() & MASK_TXREQ0) break;
	}
	if(i == 100) return -1;
	return 0;
}








void CAN_reset(void){
	SPI_SelectSlave(SPI_CAN);
	SPI_MasterTransmit(INS_RESET);
	SPI_NoSlave();
}

void CAN_read(char* a, uint8_t b , int c){

}

void CAN_read_rx(char* a, uint8_t b , int c){

}

void CAN_write(char* data, uint8_t reg, int data_count){

}

void CAN_load_tx(char* data, uint8_t tx, int data_count){
	int i;
	if (tx>2 || data_count > 8)
		return;
	uint8_t addr = 0x10*tx + TXB0D0;
	for(i = 0; i < data_count; i++){
		CAN_bit_modify(addr, 0xFF, data[i]);
	}
}

void CAN_rts(uint8_t tx){
	if (tx == 0) tx = 1;
	else if (tx == 1) tx = 2;
	else if (tx == 3) tx = 4;
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
