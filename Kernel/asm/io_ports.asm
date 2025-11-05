; Permiten leer y escribir un byte en un puerto de E/S

global outb
global inb

section .text

outb:
    mov dx, di        ; puerto
    mov al, sil       ; valor
    out dx, al
    ret

inb:
    mov dx, di
    in al, dx
    movzx rax, al
    ret