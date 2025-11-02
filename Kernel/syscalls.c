#include <stdint.h>
#include "naiveConsole.h"
#include "syscalls.h"
#include "rtc.h"
#include "videoDriver.h"
#include "lib.h"
#include "interrupts.h"

// Buffer circular para stdin
#define STDIN_BUFFER_SIZE 256
static char stdin_buffer[STDIN_BUFFER_SIZE];
static volatile uint16_t stdin_head = 0;
static volatile uint16_t stdin_tail = 0;



// Delegador principal de syscalls
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    switch (syscall_num) {
        case SYS_READ:
            return sys_read(arg1, (char *)arg2, arg3);
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, arg3);
        case SYS_GET_TIME:
            return sys_get_time((rtc_time_t*)arg1);
        case SYS_GET_DATETIME:
            return sys_get_datetime((rtc_datetime_t*)arg1);
        case SYS_GET_REGISTERS:
            return sys_get_registers((cpu_registers_t*)arg1);
        case SYS_GET_VIDEO_DATA:
            return sys_get_video_data((video_info_t*)arg1);
        case SYS_VIDEO_CLEAR:
            return sys_video_clear();
        case SYS_VIDEO_PUTPIXEL:
            return sys_video_putpixel((uint32_t)arg1, (uint32_t)arg2, (uint32_t)arg3);
        case SYS_VIDEO_DRAW_RECT:
            return sys_video_draw_rect((uint32_t)arg1, (uint32_t)arg2,
                                   (uint32_t)arg3, (uint32_t)arg4, (uint32_t)arg5);
        case SYS_PLAY_SOUND:
            return sys_play_sound((uint32_t)arg1, (uint32_t)arg2);
        default:
            return -1;  // ENOSYS
    }
}


// SYS_WRITE: Escribir a stdout/stderr
int64_t sys_write(uint64_t fd, const char *buf, uint64_t count) {
    if (fd == 1 || fd == 2) {  // stdout o stderr
        for (uint64_t i = 0; i < count; i++) {
            char c = buf[i];
            
            if (c == '\n') {
                ncNewline();
            }
            else if (c == '\b') {
                ncBackspace();
            }
            else if (c == '\t') {
                ncTab();
            }
            else if (c >= 32 && c <= 126) {
                ncPrintChar(c);
            }
            // Caracteres especiales extended ASCII (ñ, etc)
            else if (c >= 160 && c <= 255) {
                ncPrintChar(c);
            }
        }
        return count;
    }
    return -1;
}

// SYS_READ: Leer de stdin (bloqueante)
int64_t sys_read(uint64_t fd, char *buf, uint64_t count) {
    if (fd == 0) {  // stdin
        uint64_t bytes_read = 0;
        
        while (bytes_read < count) {
            // Esperar si el buffer está vacío
            while (stdin_head == stdin_tail) {
                _hlt(); // CPU duerme hasta próxima interrupción
            }
            
            // Leer del buffer circular
            buf[bytes_read] = stdin_buffer[stdin_tail];
            stdin_tail = (stdin_tail + 1) % STDIN_BUFFER_SIZE;
            
            // Si leímos newline, terminar
            if (buf[bytes_read] == '\n') {
                bytes_read++;
                break;
            }
            
            bytes_read++;
        }
        
        return bytes_read;
    }
    return -1;  // EBADF
}

// Función para que el keyboard handler agregue caracteres
void kernel_stdin_push(char c) {
    uint16_t next = (stdin_head + 1) % STDIN_BUFFER_SIZE;
    if (next != stdin_tail) {
        stdin_buffer[stdin_head] = c;
        stdin_head = next;
    }
}

// SYS_GET_TIME: Obtener tiempo actual
int64_t sys_get_time(rtc_time_t *time_ptr) {    
    rtc_read_hardware_time(time_ptr);
    return 0; 
}

// SYS_GET_DATETIME: Obtener fecha y hora completa
int64_t sys_get_datetime(rtc_datetime_t *datetime_ptr) {
    rtc_read_full_datetime(datetime_ptr);
    return 0; 
}

// SYS_GET_REGISTERS
extern void capture_registers(cpu_registers_t *regs);
int64_t sys_get_registers(cpu_registers_t *regs) {
    capture_registers(regs);
    return 0;
}

// SYS_GET_VIDEO_DATA
int64_t sys_get_video_data(video_info_t *video_info) {
    video_info->width = getScreenWidth();
    video_info->height = getScreenHeight();
    video_info->bpp = getbpp(); // Obtener bits por pixel dinámicamente
    video_info->framebuffer = 0; // Por seguridad, no exponer la dirección real
    
    return 0;
}

// NUEVA: SYS_VIDEO_CLEAR
int64_t sys_video_clear(void) {
    ncClear();  
    return 0;
}


// ---------------------------------------------------------------------------
// Syscalls de video
// ---------------------------------------------------------------------------

// Dibuja un solo píxel (usa el driver directamente)
int64_t sys_video_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= getScreenWidth() || y >= getScreenHeight())
        return -1;
    putPixel(color, x, y);
    return 0;
}

// Dibuja un rectángulo sólido
int64_t sys_video_draw_rect(uint32_t x, uint32_t y,
                            uint32_t w, uint32_t h,
                            uint32_t color) {
    uint16_t screenW = getScreenWidth();
    uint16_t screenH = getScreenHeight();

    if (x >= screenW || y >= screenH)
        return -1;

    if (x + w > screenW)
        w = screenW - x;
    if (y + h > screenH)
        h = screenH - y;

    for (uint32_t yy = y; yy < y + h; yy++) {
        for (uint32_t xx = x; xx < x + w; xx++) {
            putPixel(color, xx, yy);
        }
    }
    return 0;
}


// Stub de blit (no tenés framebuffer expuesto, así que por ahora no hace nada)
int64_t sys_video_blit(void *buf, uint64_t buf_size) {
    // Podés implementarlo luego si querés copiar desde user-space.
    return -1;
}

// ---------------------------------------------------------------------------
// Syscall de sonido (stub)
// ---------------------------------------------------------------------------
int64_t sys_play_sound(uint32_t freq, uint32_t dur_ms) {
    if (freq == 0 || dur_ms == 0)
        return -1;
    speaker_play_tone(freq, dur_ms);
    return 0;
}