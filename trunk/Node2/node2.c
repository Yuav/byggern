#include "settings.h"
#include "can.h"
#include "spi.h"

int main(void) {
	SPI_NoSlave();
	SPI_MasterInit();	
	CAN_init();

	CAN_test();

	while(1);

	return 0;	
	
}


