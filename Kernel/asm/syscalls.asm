global syscall_handler
extern handle_syscall

syscall_handler:
    pusha                  ; Guardar todos los registros
    push ds
    push es
    
    mov ax, 0x10          ; Segmento de datos del kernel
    mov ds, ax
    mov es, ax
    
    push edx              ; count
    push ecx              ; buf
    push ebx              ; fd
    push eax              ; syscall number
    
    call handle_syscall
    
    add esp, 16           ; Limpiar stack
    
    pop es
    pop ds
    popa
    iret