CC = avr-gcc
CFLAGS = -Wall
OBJ = main.o uart.o message.o gpio.o chksum.o ringbuffer.o
MCU = atmega2560
PROG = USBtiny
#DEPS = $(wildcard *.h)
TARGET = out

$(TARGET).hex: $(TARGET).bin
	avr-objcopy -j .text -j .data -O ihex $< $@

$(TARGET).bin: $(OBJ)
	$(CC) -o $@ $(OBJ) -mmcu=$(MCU) $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< -mmcu=$(MCU) $(CFLAGS)

.PHONY:clean
clean:
	rm *.o 
	rm *.hex
	rm *.bin

flash:
	avrdude -p $(MCU) -c $(PROG) -U flash:w:$(TARGET).hex:i
