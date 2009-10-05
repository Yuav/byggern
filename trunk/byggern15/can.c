#include "can.h"
#include "spi.h"

void CAN_init(void){
	CAN_reset();
	
}















void CAN_reset(void){
	SPI_SelectSlave(SPI_CAN);
	SPI_MasterTransmit(INS_RESET);

	SPI_NoSlave();
}
void CAN_read(char*, uint8_t, int){

}
void CAN_read_rx(char*, uint8_t, int){

}
void CAN_write(char*, uint8_t, int){

}
void CAN_load_tx(char*, uint8_t, int){

}
void CAN_rts(uint8_t){

}
uint8_t CAN_read_status(void){

return 0;

}
uint8_t CAN_rx_status(void){

return 0;

}
void CAN_bit_modify(uint8_t address, uint8_t mask, uint8_t data){

}
