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
    
    ; Los parámetros vienen en:
    ; rax = syscall_num (arg0)
    ; rdi = arg1
    ; rsi = arg2  
    ; rdx = arg3
    ; r10 = arg4 (en syscalls de Linux se usa r10 en lugar de rcx)
    ; r8  = arg5
    
    ; syscall_delegator espera (syscall_num, arg1, arg2, arg3, arg4, arg5)
    ; Reorganizar registros para la convención de llamada System V AMD64:
    ; rdi = syscall_num
    ; rsi = arg1
    ; rdx = arg2
    ; rcx = arg3
    ; r8  = arg4
    ; r9  = arg5
    
    mov r9, r8      ; arg5 (ya está en r8, movemos a r9)
    mov r8, r10     ; arg4 (de r10 a r8)
    mov rcx, rdx    ; arg3 (de rdx a rcx)
    mov rdx, rsi    ; arg2 (de rsi a rdx)
    mov rsi, rdi    ; arg1 (de rdi a rsi)
    mov rdi, rax    ; syscall_num (de rax a rdi)
    
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
    
    iretq  ; RAX tiene el valor de retorno