#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "time.h"

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_GET_TIME 2
#define SYS_GET_DATETIME 3
#define SYS_GET_REGISTERS 4
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


extern int64_t syscall_write(int fd, const char *buf, uint64_t count);
extern int64_t syscall_read(int fd, char *buf, uint64_t count);
extern int64_t syscall_read_nb(int fd, char *buf, uint64_t count);  // <-- agregar extern
extern int64_t syscall_get_time(void *time_ptr);
extern int64_t syscall_get_datetime(void *datetime_ptr);
extern int64_t syscall_get_registers(void *regs);
extern int64_t syscall_get_video_data(void *video_info);
extern int64_t syscall_video_clear(void);
extern int64_t sys_put_pixel(uint64_t x, uint64_t y, uint64_t color);
extern int64_t syscall_video_draw_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t color);
extern int64_t syscall_play_sound(uint64_t freq, uint64_t duration_ms);
extern int64_t syscall_increase_font_scale(void);
extern int64_t syscall_decrease_font_scale(void);
extern int64_t syscall_debug_break(void);
extern int64_t syscall_font_save_state(void);
extern int64_t syscall_font_restore_state(void);
extern int64_t syscall_get_font_width(void);
extern int64_t syscall_get_font_height(void);
extern int64_t syscall_set_text_color(uint32_t color);
extern int64_t syscall_set_background_color(uint32_t color);
extern int64_t syscall_get_ticks(void);

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

// Syscalls disponibles
int64_t write(int fd, const char *buf, uint64_t count);
int64_t read(int fd, char *buf, uint64_t count);

typedef struct {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int ticks;
} rtc_time_t;

typedef struct {
    unsigned int day;        // 1-31
    unsigned int month;      // 1-12
    unsigned int year;       // 2024
    unsigned int weekday;    // 1-7 (domingo=1)
} rtc_date_t;

typedef struct {
    rtc_time_t time;
    rtc_date_t date;
} rtc_datetime_t;

int64_t get_datetime(rtc_datetime_t *datetime_ptr);
int64_t get_time(rtc_time_t *time_ptr);

// Nuevas syscalls
int64_t get_registers(cpu_registers_t *regs);
int64_t get_video_data(video_info_t *video_info);
int64_t video_clear(void);

int64_t video_putpixel(uint32_t x, uint32_t y, uint32_t color);
int64_t video_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
int64_t play_sound(uint32_t freq, uint32_t dur_ms);
int64_t read_nb(int fd, char *buf, uint64_t count);  // no bloqueante, devuelve 0 si no hay datos
uint32_t video_getpixel(uint32_t x, uint32_t y);

int64_t increase_font_scale(void);
int64_t decrease_font_scale(void);

// Debug de emergencia
int64_t debug_break(void);

// Manejo de estado de fuente
void font_save_state(void);
void font_restore_state(void);

// Obtener tamaño de fuente actual
int64_t getFontWidth(void);
int64_t getFontHeight(void);

// Control de colores de texto
int64_t set_text_color(uint32_t color);
int64_t set_background_color(uint32_t color);

// Obtener ticks del timer
int64_t get_ticks(void);

#endif