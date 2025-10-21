global _irq00Handler
extern irqDispatcher
section .text
_irq00Handler:
    ; Guardar estado de los registros (macro pushState en tu código)
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, 0          ; pasamos como parámetro el número de IRQ (0)
    call irqDispatcher

    ; Señalar fin de interrupción al PIC
    mov al, 20h
    out 20h, al

    ; Restaurar registros
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    iretq
