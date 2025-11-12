#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "rtc.h"

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_GET_TIME 2
#define SYS_GET_DATETIME 3
#define SYS_GET_VIDEO_DATA 5
#define SYS_VIDEO_CLEAR 6
#define SYS_VIDEO_PUTPIXEL 7
#define SYS_VIDEO_DRAW_RECT 8
#define SYS_PLAY_SOUND 9
#define SYS_READ_NB 10  
#define SYS_INCREASE_FONT_SCALE 11
#define SYS_DECREASE_FONT_SCALE 12
#define SYS_VIDEO_GETPIXEL 13
#define SYS_DEBUG_BREAK 14  
#define SYS_FONT_SAVE_STATE 15
#define SYS_FONT_RESTORE_STATE 16
#define SYS_GET_FONT_WIDTH 17
#define SYS_GET_FONT_HEIGHT 18
#define SYS_SET_TEXT_COLOR 19
#define SYS_SET_BACKGROUND_COLOR 20
#define SYS_GET_TICKS 21
#define SYS_SET_CURSOR_POSITION 22
#define SYS_GET_USERLAND_REGISTERS 23  // Nueva syscall

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

int64_t sys_read(int fd, char *buf, uint64_t count);
int64_t sys_write(int fd, const char *buf, uint64_t count);
int64_t sys_get_time(rtc_time_t *time_ptr);
int64_t sys_get_datetime(rtc_datetime_t *datetime_ptr);

int64_t sys_get_video_data(video_info_t *video_info);
int64_t sys_video_clear(void);
int64_t sys_put_pixel(uint64_t x, uint64_t y, uint32_t color);
int64_t sys_video_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
int64_t sys_play_sound(uint32_t freq, uint32_t dur_ms);
int64_t sys_read_nb(int fd, char *buf, uint64_t count);
int64_t sys_increase_font_scale(void);
int64_t sys_decrease_font_scale(void);
int64_t sys_debug_break(void);
int64_t sys_debug_break_userland(void);
int64_t sys_font_save_state(void);
int64_t sys_font_restore_state(void);
int64_t sys_get_userland_registers(cpu_registers_t *regs);

int64_t sys_video_getpixel(uint32_t x, uint32_t y);
int64_t sys_font_save_state(void);
int64_t sys_font_restore_state(void);
int64_t sys_get_font_width(void);
int64_t sys_get_font_height(void);
int64_t sys_set_text_color(uint32_t color);
int64_t sys_set_background_color(uint32_t color);
int64_t sys_get_ticks(void);
int64_t sys_set_cursor_position(int x, int y);

uint32_t getCurrentTextColor(void);
uint32_t getCurrentBackgroundColor(void);

void kernel_stdin_push(unsigned char c);

// Asegúrate de que esté declarado extern
extern volatile cpu_registers_t userland_registers;

#endif