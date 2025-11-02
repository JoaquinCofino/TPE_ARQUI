# Paths
BOOTLOADER_PATH=../Bootloader
BMFS=$(BOOTLOADER_PATH)/BMFS/bmfs.bin
MBR=$(BOOTLOADER_PATH)/Pure64/bmfs_mbr.sys
MP=../Toolchain/ModulePacker/mp.bin
PURE64=$(BOOTLOADER_PATH)/Pure64/pure64.sys
KERNEL=../Kernel/kernel.bin
USERLAND=../Userland/0000-sampleCodeModule.bin ../Userland/0001-sampleDataModule.bin

# Image names
OSIMAGENAME=x64BareBonesImage
IMG=$(OSIMAGENAME).img
VMDK=$(OSIMAGENAME).vmdk
QCOW2=$(OSIMAGENAME).qcow2
PACKEDKERNEL=packedKernel.bin
IMGSIZE=6291456

# QEMU path
QEMU=qemu-system-x86_64

# Default target: build + run
all: $(IMG) $(VMDK) $(QCOW2)
	@echo "Ejecutando QEMU con pcspk..."
	$(QEMU) -soundhw pcspk -drive file=$(IMG),format=raw

# Compile kernel
$(KERNEL):
	cd ../Kernel && make

# Pack kernel + userland
$(PACKEDKERNEL): $(KERNEL) $(USERLAND)
	$(MP) $(KERNEL) $(USERLAND) -o $(PACKEDKERNEL)

# Create raw image
$(IMG): $(BMFS) $(MBR) $(PURE64) $(PACKEDKERNEL)
	$(BMFS) $(IMG) initialize $(IMGSIZE) $(MBR) $(PURE64) $(PACKEDKERNEL)

# Convert to vmdk
$(VMDK): $(IMG)
	qemu-img convert -f raw -O vmdk $(IMG) $(VMDK)

# Convert to qcow2
$(QCOW2): $(IMG)
	qemu-img convert -f raw -O qcow2 $(IMG) $(QCOW2)

# Clean build artifacts
clean:
	rm -rf $(IMG) $(VMDK) $(QCOW2) *.bin

.PHONY: all clean
