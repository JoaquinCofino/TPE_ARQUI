#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "rtc.h"

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_GET_TIME 2
#define SYS_GET_DATETIME 3
#define SYS_GET_REGISTERS 4
#define SYS_GET_VIDEO_DATA 5
#define SYS_VIDEO_CLEAR 6
#define SYS_VIDEO_PUTPIXEL 7
#define SYS_VIDEO_DRAW_RECT 8
#define SYS_PLAY_SOUND 9


// Estructuras para las nuevas syscalls
typedef struct {
    uint64_t rax, rbx, rcx, rdx;
    uint64_t rsi, rdi, rbp, rsp;
    uint64_t r8, r9, r10, r11;
    uint64_t r12, r13, r14, r15;
    uint64_t rip, rflags;
} cpu_registers_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t bpp;
    uint32_t framebuffer;
} video_info_t;

// Delegador principal
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

// Implementaciones de syscalls existentes
int64_t sys_read(int fd, char *buf, uint64_t count);
int64_t sys_write(int fd, const char *buf, uint64_t count);
int64_t sys_get_time(rtc_time_t *time_ptr);
int64_t sys_get_datetime(rtc_datetime_t *datetime_ptr);

// Nuevas syscalls
int64_t sys_get_registers(cpu_registers_t *regs);
int64_t sys_get_video_data(video_info_t *video_info);
int64_t sys_video_clear(void);
int64_t sys_put_pixel(uint64_t x, uint64_t y, uint32_t color);
int64_t sys_video_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
int64_t sys_play_sound(uint32_t freq, uint32_t dur_ms);



// Funciones auxiliares
void kernel_stdin_push(char c);

#endif