; filepath: /home/jcofino/TPE_Arqui/x64BareBones/Userland/libc/syscalls.asm
GLOBAL syscall_read
GLOBAL syscall_write
GLOBAL syscall_get_time
GLOBAL syscall_get_datetime
GLOBAL syscall_get_registers
GLOBAL syscall_get_video_data
GLOBAL syscall_video_clear
GLOBAL sys_put_pixel
GLOBAL syscall_video_draw_rect
GLOBAL syscall_play_sound
GLOBAL syscall_read_nb

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

syscall_get_registers:
    mov rax, 4
    int 0x80
    ret

syscall_get_video_data:
    mov rax, 5
    int 0x80
    ret

syscall_video_clear:
    mov rax, 6
    int 0x80
    ret
sys_put_pixel:
    mov rax, 7  
    int 0x80
    ret

syscall_video_draw_rect:
    ; Necesitas mover los parámetros a los registros correctos
    ; rdi, rsi, rdx ya tienen los primeros 3 parámetros
    ; rcx tiene el 4to parámetro, pero necesita ir a r10
    ; r8 tiene el 5to parámetro
    mov r10, rcx    ; Mover 4to parámetro de rcx a r10
    mov rax, 8      ; Número de syscall
    int 0x80
    ret

syscall_play_sound:
    mov rax, 9 
    int 0x80
    ret

    syscall_read_nb:
    mov rax, 10       ; ID para read_nb (no bloqueante)
    int 0x80
    ret