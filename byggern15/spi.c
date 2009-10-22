#include <avr/io.h>
#include "spi.h"

// Initialize the SPI Master interface
void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (DDRB & 0x0F) | ((1<<DDB5) | (1<<DDB7) | (1<<DDB3)| (1<<DDB4));
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	SPI_NoSlave();
}

// Transmit char over SPI
void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

// Recieve char over SPI
char SPI_MasterReceive(void)
{
	//send dummy char, to shift the SPDR
	SPI_MasterTransmit('@');
	
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	
	

	/* Return data register */
	return SPDR;
}

// Select SPI slave to send data to
void SPI_SelectSlave(char slave){

	/* Slave select */
	if (slave == SPI_BUTTERFLY)
		PORTB = PORTB & ~(1<<DDB3) ; // set SS for butterfly low
	else if (slave == SPI_CAN)
		PORTB = PORTB & ~(1<<DDB4) ; // set SS for CAN low 
}

// Disable chipselect on all SPI slaves (select no slave)
void SPI_NoSlave(void){
	/* Set SS high */	
	PORTB = PORTB | (1<<DDB4) | (1<<DDB3);
}
