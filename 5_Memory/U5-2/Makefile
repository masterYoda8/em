# Credits: https://www.youtube.com/watch?v=Ss9WHAg1NtY
# $(OBJCOPY) -I binary -j .eeprom -O ihex $(TARGET).bin $(TARGET).eep

MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -std=c99 -Wall -g -O2 -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I.
PORT = usb
TARGET = main
SRCS = U5-2.c

all: 
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET).bin
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 ihex $(TARGET).bin $(TARGET).eep

flash:
	avrdude -p $(MCU) -c usbasp -P $(PORT) -U flash:w:$(TARGET).hex:i -F -v -U eeprom:w:$(TARGET).eep:i

clean:
	rm -f *.bin *.hex
