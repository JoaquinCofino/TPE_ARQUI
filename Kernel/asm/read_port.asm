GLOBAL read_port
GLOBAL write_port
section .text

; uint8_t read_port(uint16_t port)
read_port:
    ; salida: al
    push rbp
    mov rbp, rsp

    mov dx, di
    in al, dx       
    movzx rax, al  

    pop rbp
    ret

; void write_port(uint16_t port, uint8_t value)
write_port:
    push rbp
    mov rbp, rsp
    
    mov dx, di          ; puerto en dx (primer parámetro)
    mov al, sil         ; valor en al (segundo parámetro: sil = parte baja de rsi)
    out dx, al          
    
    pop rbp
    ret