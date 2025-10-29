#include <stdint.h>
#include "naiveConsole.h"
#include "syscalls.h"

// Buffer circular para stdin
#define STDIN_BUFFER_SIZE 256
static char stdin_buffer[STDIN_BUFFER_SIZE];
static volatile uint16_t stdin_head = 0;
static volatile uint16_t stdin_tail = 0;

// Delegador principal de syscalls
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3) {
    switch (syscall_num) {
        case 0:  // SYS_READ
            return sys_read(arg1, (char *)arg2, arg3);
        case 1:  // SYS_WRITE
            return sys_write(arg1, (const char *)arg2, arg3);
        default:
            return -1;  // ENOSYS
    }
}

// SYS_WRITE: Escribir a stdout/stderr usando naive console
int64_t sys_write(uint64_t fd, const char *buf, uint64_t count) {
    if (fd == 1 || fd == 2) {  // stdout o stderr
        for (uint64_t i = 0; i < count; i++) {
            ncPrintChar(buf[i]);
        }
        return count;
    }
    return -1;  // EBADF
}

// SYS_READ: Leer de stdin
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