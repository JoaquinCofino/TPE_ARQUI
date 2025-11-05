Global push_all
Global pop_all
Global trigger_invalid_opcode
section .text

push_all:
    ; Guardar registros
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
    ret

pop_all:
    ; Restaurar todos los registros
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
    ret

trigger_invalid_opcode:
    db 0x0F, 0xFF   
    ret              