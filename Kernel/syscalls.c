#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>

uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    switch (syscall_num) {
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, arg3);
        case SYS_READ:
            return sys_read(arg1, (char *)arg2, arg3);
        default:
            return -1;  // ENOSYS: syscall no implementada
    }
}

int64_t sys_write(uint64_t fd, const char *buf, uint64_t count) {
    if (fd == 1 || fd == 2) {  // stdout o stderr
        for (uint64_t i = 0; i < count; i++) {
            ncPrintChar(buf[i]);
        }
        return count;
    }
    return -1;  // EBADF: bad file descriptor
}

int64_t sys_read(uint64_t fd, char *buf, uint64_t count) {
    if (fd == 0) {  // stdin
        // Implementar lectura de teclado aquí
        // Por ahora, retornar -1 (no implementado)
        return -1;
    }
    return -1;
}