; filepath: /home/jcofino/TPE_Arqui/x64BareBones/Userland/libc/syscalls.asm
GLOBAL syscall_read
GLOBAL syscall_write
GLOBAL syscall_get_time
GLOBAL syscall_get_datetime
GLOBAL syscall_is_key_pressed
GLOBAL syscall_get_registers
GLOBAL syscall_get_video_data
GLOBAL syscall_video_clear

SECTION .text

syscall_read:
    mov rax, 0
    int 0x80
    ret

syscall_write:
    mov rax, 1
    int 0x80
    ret

syscall_get_time:
    mov rax, 2
    int 0x80
    ret

syscall_get_datetime:
    mov rax, 3
    int 0x80
    ret

syscall_is_key_pressed:
    mov rax, 4
    int 0x80
    ret

syscall_get_registers:
    mov rax, 5
    int 0x80
    ret

syscall_get_video_data:
    mov rax, 6
    int 0x80
    ret

syscall_video_clear:
    mov rax, 7
    int 0x80
    ret