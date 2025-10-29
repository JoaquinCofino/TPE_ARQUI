#include "syscalls.h"

// Declarar funciones implementadas en ASM
extern int64_t syscall_read(int fd, char *buf, uint64_t count);
extern int64_t syscall_write(int fd, const char *buf, uint64_t count);

// Wrappers públicos
int64_t read(int fd, char *buf, uint64_t count) {
    return syscall_read(fd, buf, count);
}

int64_t write(int fd, const char *buf, uint64_t count) {
    return syscall_write(fd, buf, count);
}