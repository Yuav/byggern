#define SPI_CAN 's'
#define SPI_BUTTERFLY 'b'

void SPI_MasterInit(void);
void SPI_MasterTransmit(char);
void SPI_MasterTransmitString(char*);
char SPI_MasterReceive(void);
void SPI_SelectSlave(char slave);
void SPI_NoSlave(void);
