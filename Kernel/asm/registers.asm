BITS 64
GLOBAL capture_registers

SECTION .text

; void capture_registers(cpu_registers_t *regs)
capture_registers:
    ; RDI contiene el puntero a la estructura (regs)
    ; Guardar el puntero original en rax para usarlo si hace falta
    mov rax, rdi

    ; Guardar registros en la estructura (cada uno de 8 bytes)
    mov [rax + 0], rax      ; guarda el puntero en offset 0 si querés, o usar rax temporal
    ; Si querés conservar rax como temporal, primero mov qword [rax+0], rdi
    mov [rdi + 0], rax      ; (opcional) ajustá offsets según tu estructura real

    mov [rdi + 0], rax      ; rax contiene el puntero; si querés almacenar RAX real, mov rax, rax no tiene sentido
    ; Mejor: almacenar los registros reales:
    mov [rdi + 0], rax      ; (si querés guardar el puntero; si no, ver siguiente alternativa)

    ; Guardar generales correctamente: (ejemplo estándar)
    mov [rdi + 0], rax      ; (si rax tiene el valor que quieras)
    mov [rdi + 8], rbx
    mov [rdi + 16], rcx
    mov [rdi + 24], rdx
    mov [rdi + 32], rsi
    mov [rdi + 40], rdi     ; aquí guardás el puntero-argumento
    mov [rdi + 48], rbp
    mov [rdi + 56], rsp
    mov [rdi + 64], r8
    mov [rdi + 72], r9
    mov [rdi + 80], r10
    mov [rdi + 88], r11
    mov [rdi + 96], r12
    mov [rdi + 104], r13
    mov [rdi + 112], r14
    mov [rdi + 120], r15

    ; Obtener RIP real del llamador:
    call .Lget_rip
.Lget_rip:
    pop rax                 ; rax = return RIP (dirección después del CALL)
    mov [rdi + 128], rax    ; rip

    ; Obtener RFLAGS
    pushfq
    pop rax
    mov [rdi + 136], rax    ; rflags

    ret