include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

# Directorios
LIBC_DIR=../libc
CONSOLE_DIR=../Console
INCLUDE_DIR=../include

# Archivos fuente C
SOURCES=$(wildcard [^_]*.c ../tron/*.c)
LIBC_SOURCES=$(wildcard $(LIBC_DIR)/[^_]*.c)
CONSOLE_SOURCES=$(wildcard $(CONSOLE_DIR)/[^_]*.c)

# Archivo ASM de syscalls
LIBC_ASM=$(LIBC_DIR)/syscalls.asm
LIBC_ASM_OBJ=$(LIBC_DIR)/syscalls.o

# Flags de inclusión
INCLUDES=-I$(INCLUDE_DIR) -I$(LIBC_DIR) -I$(CONSOLE_DIR)

all: $(MODULE)

# Regla para compilar el archivo ASM
$(LIBC_ASM_OBJ): $(LIBC_ASM)
	nasm -felf64 $(LIBC_ASM) -o $(LIBC_ASM_OBJ)

# Compilar el módulo incluyendo el objeto ASM
$(MODULE): $(SOURCES) $(LIBC_SOURCES) $(CONSOLE_SOURCES) $(LIBC_ASM_OBJ)
	$(GCC) $(GCCFLAGS) $(INCLUDES) -T sampleCodeModule.ld _loader.c $(SOURCES) $(LIBC_SOURCES) $(CONSOLE_SOURCES) $(LIBC_ASM_OBJ) -o ../$(MODULE)

clean:
	rm -rf *.o $(LIBC_ASM_OBJ)

.PHONY: all clean