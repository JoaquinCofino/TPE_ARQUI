; push_all_no_ret.asm
GLOBAL syscall_handler
EXTERN syscall_delegator

%macro PUSH_ALL 0
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
%endmacro

%macro POP_ALL 0
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
%endmacro

section .text

syscall_handler:
    PUSH_ALL                    ; No introduce return address
    
    ; Preparar argumentos para syscall_delegator
    mov rcx, rdx                ; arg3
    mov rdx, rsi                ; arg2
    mov rsi, rdi                ; arg1
    mov rdi, rax                ; syscall_num
    
    call syscall_delegator
    
    POP_ALL                     ; No espera return address
    
    iretq                     ; Volver al contexto del usuario