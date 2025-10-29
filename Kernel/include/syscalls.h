#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define SYS_WRITE 1
#define SYS_READ  2

// Declaración del syscall delegator
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3);

// Implementaciones de syscalls
int64_t sys_write(uint64_t fd, const char *buf, uint64_t count);
int64_t sys_read(uint64_t fd, char *buf, uint64_t count);

#endif