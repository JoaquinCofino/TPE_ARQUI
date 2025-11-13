BITS 64
GLOBAL timer_capture_registers

SECTION .text

; se llama desde el timer handler para capturar el estado actual
timer_capture_registers:
    ; RDI contiene el puntero a la estructura
    push rax    ; Preservar rax temporalmente
    
    ; Guardar todos los registros generales
    mov [rdi + 0], rax      
    mov [rdi + 8], rbx      
    mov [rdi + 16], rcx     
    mov [rdi + 24], rdx     
    mov [rdi + 32], rsi     
    mov [rdi + 40], rdi     
    mov [rdi + 48], rbp     
    mov [rdi + 56], rsp     
    mov [rdi + 64], r8      
    mov [rdi + 72], r9      
    mov [rdi + 80], r10     
    mov [rdi + 88], r11     
    mov [rdi + 96], r12     
    mov [rdi + 104], r13    
    mov [rdi + 112], r14    
    mov [rdi + 120], r15    

    ; Capturar RIP aproximado (dirección de retorno)
    mov rax, [rsp + 8]      ; rip está en el stack (después del push rax)
    mov [rdi + 128], rax    ; rip

    ; Capturar RFLAGS
    pushfq
    pop rax
    mov [rdi + 136], rax    ; rflags

    ; Restaurar el valor original de rax y guardarlo
    pop rax
    mov [rdi + 0], rax      ; Guardar el valor original de rax

    ret