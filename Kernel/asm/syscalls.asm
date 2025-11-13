; syscall_handler_patched.asm
GLOBAL _syscall_handler
EXTERN syscall_delegator
EXTERN userland_registers

SECTION .text

_syscall_handler:
    ; ---------------------------
    ; Snapshot ATÓMICO de registros
    ; ---------------------------
    cli                     ; Deshabilitar IRQs para evitar snapshots parciales

    ; Guardar registros generales en userland_registers (offsets en bytes)
    mov [rel userland_registers + 0], rax
    mov [rel userland_registers + 8], rbx
    mov [rel userland_registers + 16], rcx
    mov [rel userland_registers + 24], rdx
    mov [rel userland_registers + 32], rsi
    mov [rel userland_registers + 40], rdi
    mov [rel userland_registers + 48], rbp
    mov [rel userland_registers + 56], rsp
    mov [rel userland_registers + 64], r8
    mov [rel userland_registers + 72], r9
    mov [rel userland_registers + 80], r10
    mov [rel userland_registers + 88], r11
    mov [rel userland_registers + 96], r12
    mov [rel userland_registers + 104], r13
    mov [rel userland_registers + 112], r14
    mov [rel userland_registers + 120], r15

    ; Capturar RIP desde la cima de la pila (valor empujado por INT)
    mov r11, [rsp]
    mov [rel userland_registers + 128], r11

    ; Capturar RFLAGS
    mov r11, qword [rsp + 16]
    mov [rel userland_registers + 136], r11

    sti                     ; Re-habilitar IRQs inmediatamente después del snapshot
    ; ---------------------------
    ; Fin del snapshot tempranero
    ; ---------------------------

    ; ---------------------------
    ; Guardar contexto de trabajo para la llamada a C
    ; ---------------------------
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

    ; Reordenar argumentos para la convención C (rdi, rsi, rdx, rcx, r8, r9)
    mov r9, r8      ; arg5 <= r8 (guardado por CPU antes del int)
    mov r8, r10     ; arg4 <= r10 (si tenías movs previos en wrappers)
    mov rcx, rdx    ; arg3 <= rdx
    mov rdx, rsi    ; arg2 <= rsi
    mov rsi, rdi    ; arg1 <= rdi
    mov rdi, rax    ; syscall_num <= rax

    ; Asegurar alineación de stack a 16 bytes según ABI SysV
    sub rsp, 8

    call syscall_delegator

    add rsp, 8

    ; Restaurar contexto
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
