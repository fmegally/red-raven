ODIR = ./build
IDIR = ./include
SDIR = ./src

CC = avr-gcc
CFLAGS = -Wall -I$(IDIR)
LIBS = -lm
PROG = usbtiny
OBJF = main.o uart.o protocol.o fifo.o gpio.o chksum.o 
OBJ = $(patsubst %,$(ODIR)/%, $(OBJF))
MCU = atmega328p
#DEPS = $(wildcard *.h)
TARGET = main

$(ODIR)/$(TARGET).hex: $(ODIR)/$(TARGET).bin
	avr-objcopy -j .text -j .data -O ihex $< $@

$(ODIR)/$(TARGET).bin: $(OBJ)
	$(CC) -o $@ $(OBJ) -mmcu=$(MCU) $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< -mmcu=$(MCU) $(CFLAGS)

flash:
	avrdude -p $(MCU) -c $(PROG) -U flash:w:$(ODIR)/$(TARGET).hex:i

.PHONY:clean
clean:
	echo "Removing build files .."
	@bash -c 'rm $(ODIR)/{*.o,*.hex,*.bin} && echo "Clean complete !"'
