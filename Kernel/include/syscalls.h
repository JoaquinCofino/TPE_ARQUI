#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "rtc.h"

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_GET_TIME 2
#define SYS_GET_DATETIME 3
#define SYS_IS_KEY_PRESSED 4
#define SYS_GET_REGISTERS 5
#define SYS_GET_VIDEO_DATA 6
#define SYS_VIDEO_CLEAR 7

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

// Códigos de teclas para flechas y ESC
#define KEY_ESC         1
#define KEY_ARROW_UP    72
#define KEY_ARROW_DOWN  80
#define KEY_ARROW_LEFT  75
#define KEY_ARROW_RIGHT 77

// Códigos de teclas WASD
#define KEY_W           17  // W (arriba)
#define KEY_A           30  // A (izquierda)
#define KEY_S           31  // S (abajo)
#define KEY_D           32  // D (derecha)

// Códigos adicionales útiles
#define KEY_SPACE       57  // Barra espaciadora
#define KEY_ENTER       28  // Enter
#define KEY_SHIFT_L     42  // Shift izquierdo
#define KEY_SHIFT_R     54  // Shift derecho
#define KEY_CTRL        29  // Control
#define KEY_ALT         56  // Alt

// Delegador principal
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3);

// Implementaciones de syscalls existentes
int64_t sys_read(uint64_t fd, char *buf, uint64_t count);
int64_t sys_write(uint64_t fd, const char *buf, uint64_t count);
int64_t sys_get_time(rtc_time_t *time_ptr);
int64_t sys_get_datetime(rtc_datetime_t *datetime_ptr);

// Nuevas syscalls
int64_t sys_is_key_pressed(uint64_t keycode);
int64_t sys_get_registers(cpu_registers_t *regs);
int64_t sys_get_video_data(video_info_t *video_info);
int64_t sys_video_clear(void);

// Funciones auxiliares
void kernel_stdin_push(char c);
void update_key_state(uint8_t keycode, uint8_t pressed);

#endif