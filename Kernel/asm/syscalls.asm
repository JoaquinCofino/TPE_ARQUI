GLOBAL _syscall_handler
EXTERN syscall_delegator

_syscall_handler:
    ; Guardar contexto (MENOS rax, que tiene el número de syscall)
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
    
    
    mov r9, r8      ; arg5 (ya está en r8, movemos a r9)
    mov r8, r10     ; arg4 (de r10 a r8)
    mov rcx, rdx    ; arg3 (de rdx a rcx)
    mov rdx, rsi    ; arg2 (de rsi a rdx)
    mov rsi, rdi    ; arg1 (de rdi a rsi)
    mov rdi, rax    ; syscall_num (de rax a rdi)
    
    ; Llamar al dispatcher en C
    call syscall_delegator
    ; Resultado queda en RAX 
    
    ; Restaurar 
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
    
    iretq  