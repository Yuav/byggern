#include <avr/io.h>

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = DDRB & 0xF0 | (1<<DDB3);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
		;
	/* Return data register */
	return SPDR;
}
