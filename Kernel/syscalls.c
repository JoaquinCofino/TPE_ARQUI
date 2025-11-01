#include <stdint.h>
#include "naiveConsole.h"
#include "syscalls.h"
#include "rtc.h"
#include "videoDriver.h"
#include "lib.h"

// Buffer circular para stdin
#define STDIN_BUFFER_SIZE 256
static char stdin_buffer[STDIN_BUFFER_SIZE];
static volatile uint16_t stdin_head = 0;
static volatile uint16_t stdin_tail = 0;

// Estado del teclado para tracking de teclas presionadas
static uint8_t key_states[256] = {0};

// Delegador principal de syscalls
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3) {
    switch (syscall_num) {
        case SYS_READ:
            return sys_read(arg1, (char *)arg2, arg3);
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, arg3);
        case SYS_GET_TIME:
            return sys_get_time((rtc_time_t*)arg1);
        case SYS_GET_DATETIME:
            return sys_get_datetime((rtc_datetime_t*)arg1);
        case SYS_IS_KEY_PRESSED:
            return sys_is_key_pressed(arg1);
        case SYS_GET_REGISTERS:
            return sys_get_registers((cpu_registers_t*)arg1);
        case SYS_GET_VIDEO_DATA:
            return sys_get_video_data((video_info_t*)arg1);
        case SYS_VIDEO_CLEAR:
            return sys_video_clear();
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

// SYS_IS_KEY_PRESSED
int64_t sys_is_key_pressed(uint64_t keycode) {
    if (keycode >= 256) {
        return 0; // Keycode inválido
    }
    return key_states[keycode];
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

// Función para actualizar estado de teclas (debe ser llamada desde el keyboard handler)
void update_key_state(uint8_t keycode, uint8_t pressed) {
    if (keycode < 256) {
        key_states[keycode] = pressed ? 1 : 0;
    }
}