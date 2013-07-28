SRCS = clock.c main.c mem.c systick.c sched.c svc.c semaphore.c
ASMSRCS = boot.S
LD_SCRIPT = sam3s-baremetal.ld

TARGET=out

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LD=arm-none-eabi-ld

CFLAGS  = -g3 -c -Wall --std=gnu99 -O2
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -isystem include -fomit-frame-pointer
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostdlib -ffreestanding

OBJS = $(ASMSRCS:.S=.o)
OBJS += $(SRCS:.c=.o)

.PHONY: clean proj

all: proj

again: clean all

burn:
	./flash.sh

.c.o:
	$(CC) $(CFLAGS) $*.c

.S.o:
	$(CC) $(CFLAGS) $*.S

proj: 	$(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(LD) -T$(LD_SCRIPT) $(OBJS) -o $(TARGET).elf
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -j flash -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o *.i *.s
	rm -f *.elf
	rm -f *.hex
	rm -f *.bin
