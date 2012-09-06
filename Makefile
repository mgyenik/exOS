#SRCS = #start.c main.c 
ASMSRCS = start.S
LD_SCRIPT = stm-baremetal.ld

TARGET=main

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LD=arm-none-eabi-ld

CFLAGS  = -g3 -c -Wall --std=gnu99
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostdlib -ffreestanding
CFLAGS += -save-temps

#OBJS = $(SRCS:.c=.o)
OBJS = $(ASMSRCS:.S=.o)

.PHONY: proj

all: proj

again: clean all

burn:
	st-flash write $(TARGET).bin 0x8000000

.c.o:
	$(CC) $(CFLAGS) $*.c
.S.o:
	$(CC) $(CFLAGS) $*.S

proj: 	$(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(LD) -T$(LD_SCRIPT) $(OBJS) -o $(TARGET).elf
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o *.i *.s
	rm -f *.elf
	rm -f *.hex
	rm -f *.bin
