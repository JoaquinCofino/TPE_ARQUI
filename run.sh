#!/bin/bash

# Script universal para ejecutar QEMU con audio
# Compatible con QEMU < 7 y >= 7

QEMU=qemu-system-x86_64
IMAGE=Image/PROBAR_x64BareBonesImage.qcow2
MEMORY=512

# Detectar versión de QEMU
QEMU_VERSION=$($QEMU --version 2>/dev/null | head -n1 | grep -oP '\d+\.\d+' | head -n1)
QEMU_MAJOR=$(echo $QEMU_VERSION | cut -d. -f1)


# Configurar flags de audio según la versión
if [ "$QEMU_MAJOR" -ge 7 ] 2>/dev/null; then
    AUDIO_FLAGS="-machine pcspk-audiodev=audio0 -audiodev id=audio0,driver=pa"
else
    AUDIO_FLAGS="-soundhw pcspk"
fi


# Ejecutar QEMU
$QEMU -hda $IMAGE -m $MEMORY $AUDIO_FLAGS

# Si falla, ofrecer ejecutar sin audio
if [ $? -ne 0 ]; then
    echo ""
    echo "Error al ejecutar con audio."
    read -p "¿Intentar sin audio? (s/n): " choice
    if [ "$choice" = "s" ] || [ "$choice" = "S" ]; then
        $QEMU -hda $IMAGE -m $MEMORY
    fi
fi