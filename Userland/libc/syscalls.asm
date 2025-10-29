GLOBAL syscall_read
GLOBAL syscall_write

SECTION .text

; int64_t syscall_read(int fd, char *buf, uint64_t count)
; Argumentos: RDI=fd, RSI=buf, RDX=count
syscall_read:
    mov rax, 0          ; SYS_READ
    int 0x80            ; Llamar al kernel
    ret                 ; Resultado en RAX

; int64_t syscall_write(int fd, const char *buf, uint64_t count)
syscall_write:
    mov rax, 1          ; SYS_WRITE
    int 0x80
    ret