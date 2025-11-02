# Paths
BOOTLOADER_PATH=../Bootloader
BMFS=$(BOOTLOADER_PATH)/BMFS/bmfs.bin
MBR=$(BOOTLOADER_PATH)/Pure64/bmfs_mbr.sys
MP=../Toolchain/ModulePacker/mp.bin
PURE64=$(BOOTLOADER_PATH)/Pure64/pure64.sys
OSIMAGENAME=x64BareBonesImage
VMDK=$(OSIMAGENAME).vmdk
QCOW2=$(OSIMAGENAME).qcow2
IMG=$(OSIMAGENAME).img
KERNEL=../Kernel/kernel.bin
USERLAND=../Userland/0000-sampleCodeModule.bin ../Userland/0001-sampleDataModule.bin

PACKEDKERNEL=packedKernel.bin
IMGSIZE=6291456

# QEMU configuration
QEMU=qemu-system-x86_64

# Detectar versión de QEMU
QEMU_VERSION := $(shell $(QEMU) --version 2>/dev/null | head -n1 | grep -oP '\d+\.\d+' | head -n1)
QEMU_MAJOR := $(shell echo $(QEMU_VERSION) | cut -d. -f1)

# Configurar flags de audio según la versión
ifeq ($(shell test $(QEMU_MAJOR) -ge 7; echo $$?),0)
    # QEMU >= 7: sintaxis moderna
    QEMU_AUDIO_FLAGS=-machine pcspk-audiodev=audio0 -audiodev id=audio0,driver=pa
    AUDIO_MSG=QEMU >= 7 detectado - usando sintaxis moderna de audio
else
    # QEMU < 7: sintaxis antigua
    QEMU_AUDIO_FLAGS=-soundhw pcspk
    AUDIO_MSG=QEMU < 7 detectado - usando sintaxis antigua de audio
endif

# Default target: build all images
all: $(IMG) $(VMDK) $(QCOW2)

# Target para compilar y ejecutar
run: $(IMG)
	@echo "=========================================="
	@echo "$(AUDIO_MSG)"
	@echo "Versión QEMU: $(QEMU_VERSION)"
	@echo "=========================================="
	$(QEMU) $(QEMU_AUDIO_FLAGS) -drive file=$(IMG),format=raw

# Target alternativo sin audio (por si hay problemas)
run-nosound: $(IMG)
	@echo "Ejecutando QEMU SIN audio..."
	$(QEMU) -drive file=$(IMG),format=raw

# Compile kernel
$(KERNEL):
	cd ../Kernel && make

# Pack kernel + userland
$(PACKEDKERNEL): $(KERNEL) $(USERLAND)
	$(MP) $(KERNEL) $(USERLAND) -o $(PACKEDKERNEL)

# Create raw image
$(IMG): $(BMFS) $(MBR) $(PURE64) $(PACKEDKERNEL)
	$(BMFS) $(IMG) initialize $(IMGSIZE) $(MBR) $(PURE64) $(PACKEDKERNEL)

# Convert raw image to VMDK
$(VMDK): $(IMG)
	qemu-img convert -f raw -O vmdk $(IMG) $(VMDK)

# Convert raw image to QCOW2
$(QCOW2): $(IMG)
	qemu-img convert -f raw -O qcow2 $(IMG) $(QCOW2)

# Clean all generated files
clean:
	rm -rf $(IMG) $(VMDK) $(QCOW2) *.bin

# Show detected QEMU version and audio flags
info:
	@echo "QEMU Version: $(QEMU_VERSION)"
	@echo "QEMU Major: $(QEMU_MAJOR)"
	@echo "Audio Flags: $(QEMU_AUDIO_FLAGS)"

.PHONY: all clean run run-nosound info