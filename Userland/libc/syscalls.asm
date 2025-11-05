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
GLOBAL syscall_increase_font_scale
GLOBAL syscall_decrease_font_scale
GLOBAL sys_video_getpixel
GLOBAL syscall_debug_break
GLOBAL syscall_font_save_state
GLOBAL syscall_font_restore_state
GLOBAL syscall_get_font_width
GLOBAL syscall_get_font_height
GLOBAL syscall_set_text_color
GLOBAL syscall_set_background_color
GLOBAL syscall_get_ticks
GLOBAL syscall_set_cursor_position

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
    mov r10, rcx  
    mov rax, 8     
    int 0x80
    ret

syscall_play_sound:
    mov rax, 9 
    int 0x80
    ret

syscall_read_nb:
    mov rax, 10       
    int 0x80
    ret

syscall_increase_font_scale:
    mov rax, 11
    int 0x80
    ret

syscall_decrease_font_scale:
    mov rax, 12
    int 0x80
    ret

sys_video_getpixel:
    mov rax, 13 
    int 0x80
    ret

syscall_debug_break:
    mov rax, 14       
    int 0x80
    ret

syscall_font_save_state:
    mov rax, 15      
    int 0x80
    ret

syscall_font_restore_state:
    mov rax, 16     
    int 0x80
    ret

syscall_get_font_width:
    mov rax, 17      
    int 0x80
    ret

syscall_get_font_height:
    mov rax, 18    
    int 0x80
    ret

syscall_set_text_color:
    mov rax, 19       
    int 0x80
    ret

syscall_set_background_color:
    mov rax, 20    
    int 0x80
    ret

syscall_get_ticks:
    mov rax, 21      
    int 0x80
    ret


syscall_set_cursor_position:
    mov rax, 22       
    int 0x80
    ret

global trigger_invalid_opcode
trigger_invalid_opcode:
    db 0x0F, 0xFF    
    ret            