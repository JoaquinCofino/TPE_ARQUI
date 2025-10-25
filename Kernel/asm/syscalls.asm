global syscall_handler
extern push_all
extern pop_all
extern syscall_delegator

syscall_handler:
    call push_all ; Guardar todos los registros

    ; Los parámetros se pasan en registros:
        ; syscall number → rdi
        ; fd → rsi
        ; buf → rdx
        ; count → rcx
        mov rdi, rax        ; syscall number (venía en rax)
        mov rsi, rbx        ; fd
        mov rdx, rcx        ; buf
        mov rcx, rdx        ; count (ajustar si la llamada original usa otro orden)

        ; Llamar a handle_syscall
        call syscall_delegator

    ; Restaurar registros
    call pop_all

    iret