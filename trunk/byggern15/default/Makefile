###############################################################################
# Makefile for the project bygger_gruppe_15
###############################################################################

## General Flags
PROJECT = bygger_gruppe_15
MCU = atmega162
TARGET = bygger_gruppe_15.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=bygger_gruppe_15.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = bygger_gruppe_15.o uart.o sram.o joystick.o spi.o can.o butterfly_interface.o game.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) bygger_gruppe_15.hex bygger_gruppe_15.eep bygger_gruppe_15.lss size

## Compile
bygger_gruppe_15.o: ../bygger_gruppe_15.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sram.o: ../sram.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

joystick.o: ../joystick.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

can.o: ../can.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

butterfly_interface.o: ../butterfly_interface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

game.o: ../game.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) bygger_gruppe_15.elf dep/* bygger_gruppe_15.hex bygger_gruppe_15.eep bygger_gruppe_15.lss bygger_gruppe_15.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

