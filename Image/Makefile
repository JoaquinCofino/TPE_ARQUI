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

# Default target: build all images
all: $(IMG) $(VMDK) $(QCOW2)

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

.PHONY: all clean
