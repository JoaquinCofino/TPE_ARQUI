#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "rtc.h"

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_GET_TIME 2
#define SYS_GET_DATETIME 3

#include <stdint.h>

// Delegador principal
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, 
                          uint64_t arg2, uint64_t arg3);

// Implementaciones de syscalls
int64_t sys_read(uint64_t fd, char *buf, uint64_t count);
int64_t sys_write(uint64_t fd, const char *buf, uint64_t count);
int64_t sys_get_time(rtc_time_t *time_ptr);
int64_t sys_get_datetime(rtc_datetime_t *datetime_ptr);


// Función para que el driver de teclado agregue input
void kernel_stdin_push(char c);

#endif