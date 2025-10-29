#ifndef USERLAND_SYSCALLS_H
#define USERLAND_SYSCALLS_H

#include <stdint.h>

#define SYS_WRITE 1
#define SYS_READ  2

// API pública para userland
int64_t read(int fd, char *buf, uint64_t count);
int64_t write(int fd, const char *buf, uint64_t count);

#endif