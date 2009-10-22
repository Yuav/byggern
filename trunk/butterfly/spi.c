#include <avr/io.h>

// Initialize SPI as slave
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = DDRB & 0xF0 | (1<<DDB3);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

// Receive data over SPI
char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
		;
	/* Return data register */
	return SPDR;
}

// Send data over SPI
void SPI_SlaveTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
		;
}
