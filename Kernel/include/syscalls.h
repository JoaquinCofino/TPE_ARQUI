#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3);
int64_t sys_write(int fd, const char *buf, uint64_t count);

#endif