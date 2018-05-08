#
# Makefile for compiling a FreeRTOS/uIP system for RX63N
# using GNURX
#
# target
#  all : make elf and bin rom files
#  flash : flash and run
#  debug : start gdb debugger (JLinkGDBServer must be started)
#  vdebug : attaches nemiver to the current gdb-remote-server
#

## Tools configuration with gnurx
TOOL_PATH=~/toolchains/gcc_gnurx
JLINK_PATH=/usr/bin

CC=$(TOOL_PATH)/bin/rx-elf-gcc
AS=$(TOOL_PATH)/bin/rx-elf-as
LD=$(TOOL_PATH)/bin/rx-elf-ld
LIBGEN=$(TOOL_PATH)/bin/rx-elf-libgen
OBJCOPY=$(TOOL_PATH)/bin/rx-elf-objcopy
OBJDUMP=$(TOOL_PATH)/bin/rx-elf-objdump
GDB=$(TOOL_PATH)/bin/rx-elf-gdb
JLINK_GDB_SERVER=$(JLINK_PATH)/JLinkGDBServer 
JLINK_EXE=$(JLINK_PATH)/JLinkExe 


# we take all the sources from the src directory and make objects
C_SRC=$(shell find src -name "*.c"|grep -v \\/\\.)
C_OBJ=$(C_SRC:%.c=%.o)
ASM_SRC=$(shell find src -name "*.asm"|grep -v \\/\\.)
ASM_OBJ=$(ASM_SRC:%.asm=%.o)
OBJ=$(C_OBJ) $(ASM_OBJ)

# include directories
INCDIR+=-Isrc
INCDIR+=-Isrc/asservissement
INCDIR+=-Isrc/detection
INCDIR+= -Isrc/system
INCDIR+= -Isrc/drivers/uart
INCDIR+= -Isrc/drivers/adc
INCDIR+= -Isrc/drivers/i2c
INCDIR+= -Isrc/drivers/pwm
INCDIR+= -Isrc/drivers/decoder_quadra
INCDIR+= -Isrc/r_freertos
INCDIR+= -Isrc/r_freertos/source/include
INCDIR+= -Isrc/r_freertos/source/portable/RX200

CFLAGS= -g $(INCDIR) -O2 -std=c11 -nofpu -mcpu=rx200 -Wall
#LDFLAGS= -L$(TOOL_PATH)/lib/gcc/rx-elf/4.8.4.201801-GNURX/no-fpu-libs/ -lgcc 
#LDFLAGS+= -L$(TOOL_PATH)/rx-elf/lib/ -lm -lc

LIBRARY=lib.a libgcc.a

LINKER_SCRIPT=scripts/FreeRTOS_rx210.gsi

all: rom.bin

%.o:%.asm
	$(AS) $< -o $@

lib.a:
	$(LIBGEN) --select-lib=optlib --header-files=all --compiler-options=-mcpu=rx200,-mlittle-endian-data,-O2 --assembler-options=--mlittle-endian-data -o "lib.a"	

rom.x : $(OBJ) lib.a
	$(LD) -o $@ -T $(LINKER_SCRIPT) $(OBJ) $(LIBRARY) $(LDFLAGS) -e _PowerON_Reset 
#-M=HardwareDebug/FreeRTOSIIC.map

rom.bin : rom.x
	$(OBJCOPY) -O binary rom.x rom.bin	

flash : rom.bin
	$(JLINK_EXE) scripts/jlink_flash -Device RX210 -if FINE 

debug : rom.x
	$(GDB) -x scripts/gdbinit

ddd : rom.x
	ddd --debugger '$(GDB) -x scripts/gdbinit'

dump : rom.x
	$(OBJDUMP) -D rom.x

gdbserver : 
	$(JLINK_GDB_SERVER) -device RX210 -if FINE

clean:
	rm -rf $(OBJ) rom.x rom.bin
	
