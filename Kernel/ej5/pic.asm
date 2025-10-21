global picMasterMask
global picSlaveMask

section .text
picMasterMask:
    mov al, dil        ; dil = primer argumento en 64 bits (uint8_t mask)
    out 0x21, al
    ret

picSlaveMask:
    mov al, dil
    out 0xA1, al
    ret
