COMPILER = gcc
LINKER = ld
ASSEMBLER = nasm
CFLAGS = -m32 -c -ffreestanding
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T scr/link.ld
EMULATOR = qemu-system-i386
EMULATOR_FLAGS = -kernel

OBJS = temp/obj/kasm.o temp/obj/kc.o
OUTPUT = nos/kernel.bin

all: kernel 

kernel: functions
	$(ASSEMBLER) $(ASFLAGS) -o temp/obj/kasm.o scr/kernel.asm
	$(COMPILER) $(CFLAGS) scr/kernel.c -o temp/obj/kc.o 
	$(LINKER) $(LDFLAGS) -o $(OUTPUT) $(OBJS)
	
functions:
	mkdir temp/obj -p

test:
	$(EMULATOR) $(EMULATOR_FLAGS) $(OUTPUT)