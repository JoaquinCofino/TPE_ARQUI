GLOBAL capture_registers

SECTION .text

; void capture_registers(cpu_registers_t *regs)
capture_registers:
    ; RDI contiene el puntero a la estructura
    
    ; Guardar registros en la estructura (cada uno de 8 bytes)
    mov [rdi + 0], rax      ; rax
    mov [rdi + 8], rbx      ; rbx
    mov [rdi + 16], rcx     ; rcx
    mov [rdi + 24], rdx     ; rdx
    mov [rdi + 32], rsi     ; rsi
    mov [rdi + 40], rdi     ; rdi (valor original)
    mov [rdi + 48], rbp     ; rbp
    mov [rdi + 56], rsp     ; rsp
    mov [rdi + 64], r8      ; r8
    mov [rdi + 72], r9      ; r9
    mov [rdi + 80], r10     ; r10
    mov [rdi + 88], r11     ; r11
    mov [rdi + 96], r12     ; r12
    mov [rdi + 104], r13    ; r13
    mov [rdi + 112], r14    ; r14
    mov [rdi + 120], r15    ; r15
    
    ; Para RIP y RFLAGS necesitamos valores aproximados
    mov rax, capture_registers  ; Dirección aproximada de RIP
    mov [rdi + 128], rax        ; rip
    
    pushfq                      ; Obtener RFLAGS
    pop rax
    mov [rdi + 136], rax        ; rflags
    
    ret