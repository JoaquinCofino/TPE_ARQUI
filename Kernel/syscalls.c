#include <stdint.h>
#include "include/naiveConsole.h"
#include "include/syscalls.h"
#include "include/rtc.h"
#include "include/videoDriver.h"
#include "include/interrupts.h"
#include "include/speaker.h"

// Buffer circular para stdin
#define STDIN_BUFFER_SIZE 256
static unsigned char stdin_buffer[STDIN_BUFFER_SIZE];
static volatile uint16_t stdin_head = 0;
static volatile uint16_t stdin_tail = 0;

// Variables globales - MANTENER SOLO UNA DEFINICIÓN DE CADA UNA
static uint8_t saved_font_scale = 1;
static uint32_t current_text_color = 0xFFFFFF;      // Blanco por defecto
static uint32_t current_background_color = 0x000000; // Negro por defecto

volatile cpu_registers_t userland_registers = {0};  // Nueva variable para registros de userland

// Delegador principal de syscalls
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    
    // Capturar registros de userland cada vez que se hace una syscall
    timer_capture_registers((cpu_registers_t*)&userland_registers);
    
    switch (syscall_num) {
        case SYS_READ:
            return sys_read(arg1, (char *)arg2, arg3);
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, arg3);
        case SYS_GET_TIME:
            return sys_get_time((rtc_time_t*)arg1);
        case SYS_GET_DATETIME:
            return sys_get_datetime((rtc_datetime_t*)arg1);
        case SYS_GET_VIDEO_DATA:
            return sys_get_video_data((video_info_t*)arg1);
        case SYS_VIDEO_CLEAR:
            return sys_video_clear();
        case SYS_VIDEO_PUTPIXEL:
            return sys_put_pixel((uint32_t)arg1, (uint32_t)arg2, (uint32_t)arg3);
        case SYS_VIDEO_DRAW_RECT:
            return sys_video_draw_rect((uint32_t)arg1, (uint32_t)arg2, (uint32_t)arg3, (uint32_t)arg4, (uint32_t)arg5);
        case SYS_PLAY_SOUND:
            return sys_play_sound((uint32_t)arg1, (uint32_t)arg2);
        case SYS_READ_NB: 
            return sys_read_nb((int)arg1, (char*)arg2, (uint64_t)arg3);
        case SYS_INCREASE_FONT_SCALE:
            return sys_increase_font_scale();
        case SYS_DECREASE_FONT_SCALE:
            return sys_decrease_font_scale();  
        case SYS_VIDEO_GETPIXEL:
            return sys_video_getpixel((uint32_t)arg1, (uint32_t)arg2);  
        case SYS_DEBUG_BREAK:
            return sys_debug_break();
        case SYS_FONT_SAVE_STATE:
            return sys_font_save_state();
        case SYS_FONT_RESTORE_STATE:
            return sys_font_restore_state();
        case SYS_GET_FONT_WIDTH:
            return sys_get_font_width();
        case SYS_GET_FONT_HEIGHT:
            return sys_get_font_height();
        case SYS_SET_TEXT_COLOR:
            return sys_set_text_color((uint32_t)arg1);
        case SYS_SET_BACKGROUND_COLOR:
            return sys_set_background_color((uint32_t)arg1);
        case SYS_GET_TICKS:
            return sys_get_ticks();
        case SYS_SET_CURSOR_POSITION:
            return sys_set_cursor_position((int)arg1, (int)arg2);
        case SYS_GET_USERLAND_REGISTERS:
            return sys_get_userland_registers((cpu_registers_t*)arg1);
        default:
            return -1;
    }
}

// SYS_WRITE: Escribir a stdout/stderr
int64_t sys_write(int fd, const char *buf, uint64_t count) {
    if (fd == 1 || fd == 2) {  // stdout o stderr
        for (uint64_t i = 0; i < count; i++) {
            unsigned char c = (unsigned char)buf[i];
            
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
int64_t sys_read(int fd, char *buf, uint64_t count) {
    if (fd == 0) {  // stdin
        uint64_t bytes_read = 0;
        
        while (bytes_read < count) {
            // Esperar si el buffer está vacío
            while (stdin_head == stdin_tail) {
                _hlt(); // CPU duerme hasta próxima interrupción
            }
            
            // Leer del buffer circular
            unsigned char c = stdin_buffer[stdin_tail];
            buf[bytes_read] = (char)c;
            stdin_tail = (stdin_tail + 1) % STDIN_BUFFER_SIZE;
            
            // Si leímos newline, terminar
            if (c == '\n') {
                bytes_read++;
                break;
            }
            
            bytes_read++;
        }
        
        return bytes_read;
    }
    return -1;  // EBADF
}

// SYS_READ_NB: Leer de stdin sin bloquear (devuelve 0 si no hay datos)
int64_t sys_read_nb(int fd, char *buf, uint64_t count) {
    if (fd != 0 || buf == 0 || count == 0) return -1;

    uint64_t bytes_read = 0;

    // Leer lo disponible sin esperar
    while (bytes_read < count) {
        if (stdin_head == stdin_tail) {
            // No hay más datos disponibles ahora
            break;
        }
        unsigned char c = stdin_buffer[stdin_tail];
        buf[bytes_read] = (char)c;
        stdin_tail = (stdin_tail + 1) % STDIN_BUFFER_SIZE;
        bytes_read++;
    }

    return bytes_read;  // 0 si no había datos
}

// Función para que el keyboard handler agregue caracteres
void kernel_stdin_push(unsigned char c) {
    unsigned char next = (stdin_head + 1) % STDIN_BUFFER_SIZE;
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



// SYS_GET_VIDEO_DATA
int64_t sys_get_video_data(video_info_t *video_info) {
    video_info->width = getScreenWidth();
    video_info->height = getScreenHeight();
    video_info->bpp = getbpp(); // Obtener bits por pixel dinámicamente
    video_info->framebuffer = 0; // Por seguridad, no exponer la dirección real
    
    return 0;
}

int64_t sys_video_clear(void) {
    ncClear();  
    return 0;
}


int64_t sys_play_sound(uint32_t freq, uint32_t dur_ms) {
    if (freq == 0 || dur_ms == 0)
        return -1;
    speaker_play_tone(freq, dur_ms);
    return 0;
}

int64_t sys_put_pixel(uint64_t x, uint64_t y, uint32_t color) {
    uint16_t w = getScreenWidth();
    uint16_t h = getScreenHeight();
    if (x >= w || y >= h) return -1;

    uint8_t *fb = (uint8_t *)getFramebuffer();
    uint32_t bpp = getbpp() / 8;
    uint32_t pitch = getFramebufferPitch();

    uint64_t offset = y * pitch + x * bpp;
    for (uint32_t i = 0; i < bpp; i++) {
        fb[offset + i] = (color >> (8 * i)) & 0xFF;
    }
    return 0;
}

int64_t sys_video_draw_rect(uint32_t x, uint32_t y,
                            uint32_t w, uint32_t h,
                            uint32_t color) {
    uint16_t screenW = getScreenWidth();
    uint16_t screenH = getScreenHeight();

    // Verificar si el rectángulo está completamente fuera de la pantalla
    if (x >= screenW || y >= screenH) {
        return -1;
    }

    // Ajustar las dimensiones para que no se salgan de la pantalla
    uint32_t max_width = screenW - x;
    uint32_t max_height = screenH - y;
    
    // Limitar el ancho y alto al espacio disponible
    if (w > max_width) {
        w = max_width;
    }
    if (h > max_height) {
        h = max_height;
    }

    // Dibujar el rectángulo pixel por pixel usando putPixel
    for (uint32_t row = 0; row < h; row++) {
        for (uint32_t col = 0; col < w; col++) {
            putPixel(color, x + col, y + row);
        }
    }

    return 0;
}

int64_t sys_increase_font_scale(void) {
    increaseFontScale();
    return 0;
}

int64_t sys_decrease_font_scale(void) {
    decreaseFontScale();
    return 0;
}

int64_t sys_video_getpixel(uint32_t x, uint32_t y) {
    if (x >= getScreenWidth() || y >= getScreenHeight())
        return 0x000000;  // Negro si está fuera de rango
    return getPixel(x, y);  
}

// Debug de emergencia imprime registros y pausa temporalmente
int64_t sys_debug_break(void) {
    extern void ncPrint(const char*);
    extern void ncNewline(void);
    extern void ncPrintHex(uint64_t);

    // Limpiar línea actual y mostrar mensaje de debug
    ncNewline();
    ncPrint("=== USERLAND REGS (Ctrl+R) ===");
    ncNewline();

    // Mostrar registros capturados de userland
    cpu_registers_t regs;
    if (sys_get_userland_registers(&regs) == 0) {
        // Imprimir todos los registros en una línea secuencial separados por ;
        ncPrint("RAX:0x"); ncPrintHex(regs.rax); ncPrint(";");
        ncPrint("RBX:0x"); ncPrintHex(regs.rbx); ncPrint(";");
        ncPrint("RCX:0x"); ncPrintHex(regs.rcx); ncPrint(";");
        ncPrint("RDX:0x"); ncPrintHex(regs.rdx); ncPrint(";");
        ncNewline();

        ncPrint("RSI:0x"); ncPrintHex(regs.rsi); ncPrint(";");
        ncPrint("RDI:0x"); ncPrintHex(regs.rdi); ncPrint(";");
        ncPrint("RBP:0x"); ncPrintHex(regs.rbp); ncPrint(";");
        ncPrint("RSP:0x"); ncPrintHex(regs.rsp); ncPrint(";");
        ncNewline();

        ncPrint("R8:0x"); ncPrintHex(regs.r8); ncPrint(";");
        ncPrint("R9:0x"); ncPrintHex(regs.r9); ncPrint(";");
        ncPrint("R10:0x"); ncPrintHex(regs.r10); ncPrint(";");
        ncPrint("R11:0x"); ncPrintHex(regs.r11); ncPrint(";");
        ncNewline();

        ncPrint("R12:0x"); ncPrintHex(regs.r12); ncPrint(";");
        ncPrint("R13:0x"); ncPrintHex(regs.r13); ncPrint(";");
        ncPrint("R14:0x"); ncPrintHex(regs.r14); ncPrint(";");
        ncPrint("R15:0x"); ncPrintHex(regs.r15); ncPrint(";");
        ncNewline();

        ncPrint("RIP:0x"); ncPrintHex(regs.rip); ncPrint(";");
        ncPrint("RFLAGS:0x"); ncPrintHex(regs.rflags); ncPrint(";");
        ncNewline();
    }

    ncPrint("==============================");
    ncNewline();

    return 0;
}


int64_t sys_font_save_state(void) {
    extern uint8_t getFontScale(void);
    saved_font_scale = getFontScale();
    return 0;
}

int64_t sys_font_restore_state(void) {
    extern void setFontScale(uint8_t scale);
    setFontScale(saved_font_scale);
    return 0;
}

int64_t sys_get_font_width(void) {
    return (int64_t)getFontWidth();
}

int64_t sys_get_font_height(void) {
    return (int64_t)getFontHeight();
}


int64_t sys_set_text_color(uint32_t color) {
    current_text_color = color;
    return 0;
}

int64_t sys_set_background_color(uint32_t color) {
    current_background_color = color;
    return 0;
}

// Funciones para obtener los colores actuales 
uint32_t getCurrentTextColor(void) {
    return current_text_color;
}

uint32_t getCurrentBackgroundColor(void) {
    return current_background_color;
}

int64_t sys_get_ticks(void) {
    extern unsigned int timer_get_ticks(void);
    return (int64_t)timer_get_ticks();
}

int64_t sys_set_cursor_position(int x, int y) {
    ncSetCursorPosition(x, y);
    return 0;
}

// Nueva implementación para obtener registros de userland
int64_t sys_get_userland_registers(cpu_registers_t *regs) {
    if (regs == 0) return -1;
    *regs = userland_registers;
    return 0;
}