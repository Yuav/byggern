###############################################################################
# Makefile for the project Node2
###############################################################################

## General Flags
PROJECT = Node2
MCU = atmega128
TARGET = Node2.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Node2.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = can.o mcp2515.o spi.o node2.o servo.o ir.o solenoid.o motor.o TWI_Master.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Node2.hex Node2.eep Node2.lss size

## Compile
can.o: ../can.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcp2515.o: ../mcp2515.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

spi.o: ../spi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

node2.o: ../node2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

servo.o: ../servo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ir.o: ../ir.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

solenoid.o: ../solenoid.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

motor.o: ../motor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TWI_Master.o: ../TWI_Master.c
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
	-rm -rf $(OBJECTS) Node2.elf dep/* Node2.hex Node2.eep Node2.lss Node2.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

