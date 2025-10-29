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
    
    ; Los parámetros ya vienen en:
    ; rax = syscall_num (arg0)
    ; rdi = arg1
    ; rsi = arg2  
    ; rdx = arg3
    ; Pero syscall_delegator espera (rdi, rsi, rdx, rcx)
    ; Necesitamos mover rax a rdi
    
    mov rcx, rdx    ; arg3
    mov rdx, rsi    ; arg2
    mov rsi, rdi    ; arg1
    mov rdi, rax    ; syscall_num
    
    ; Llamar al dispatcher en C
    call syscall_delegator
    ; Resultado queda en RAX - NO lo toques
    
    ; Restaurar contexto (sin tocar rax)
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
    
    iretq  ; RAX tiene el valor de retorno