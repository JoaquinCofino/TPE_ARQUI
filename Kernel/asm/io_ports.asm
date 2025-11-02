; ------------------------------------------------------
; io_ports.asm - Rutinas de E/S para puertos (x86)
; ------------------------------------------------------
; Permiten leer y escribir un byte en un puerto de E/S
; Usadas por el driver del speaker, PIT, teclado, etc.
; ------------------------------------------------------

global outb
global inb

section .text

; void outb(uint16_t port, uint8_t value)
; Argumentos (x86-64 SysV ABI):
;   DI = port
;   SIL = value
outb:
    mov dx, di        ; puerto
    mov al, sil       ; valor
    out dx, al
    ret

; uint8_t inb(uint16_t port)
; Argumento:
;   DI = port
; Retorna:
;   AL = valor leído, extendido a RAX
inb:
    mov dx, di
    in al, dx
    movzx rax, al
    ret