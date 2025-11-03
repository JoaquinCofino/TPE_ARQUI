; filepath: /home/jcofino/TPE_Arqui/x64BareBones/Kernel/asm/registers.asm
BITS 64
GLOBAL timer_capture_registers

SECTION .text

; void timer_capture_registers(cpu_registers_t *regs)
; Esta función se llama desde el timer handler para capturar el estado actual
timer_capture_registers:
    ; RDI contiene el puntero a la estructura
    push rax    ; Preservar rax temporalmente
    
    ; Guardar todos los registros generales
    mov [rdi + 0], rax      ; rax (será sobrescrito con el valor correcto después)
    mov [rdi + 8], rbx      ; rbx
    mov [rdi + 16], rcx     ; rcx
    mov [rdi + 24], rdx     ; rdx
    mov [rdi + 32], rsi     ; rsi
    mov [rdi + 40], rdi     ; rdi (el puntero que recibimos)
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