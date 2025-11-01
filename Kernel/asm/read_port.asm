GLOBAL read_port
GLOBAL write_port
section .text

; uint8_t read_port(uint16_t port)
read_port:
    ; entrada: port en dx (si es el scancode del teclado, debería ser 0x60)
    ; salida: al
    push rbp
    mov rbp, rsp

    mov dx, di
    in al, dx       
    movzx rax, al   ; extender a 64 bits para retorno

    pop rbp
    ret

; void write_port(uint16_t port, uint8_t value)
write_port:
    push rbp
    mov rbp, rsp
    
    mov dx, di          ; puerto en dx (primer parámetro)
    mov al, sil         ; valor en al (segundo parámetro: sil = parte baja de rsi)
    out dx, al          ; escribir byte al puerto
    
    pop rbp
    ret