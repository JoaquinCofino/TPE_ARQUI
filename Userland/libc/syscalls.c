#include "syscalls.h"

extern int64_t syscall_write(int fd, const char *buf, uint64_t count);
extern int64_t syscall_read(int fd, char *buf, uint64_t count);
extern int64_t syscall_get_time(void *time_ptr);
extern int64_t syscall_get_datetime(void *datetime_ptr);  // ← Agregar

int64_t write(int fd, const char *buf, uint64_t count) {
    return syscall_write(fd, buf, count);
}

int64_t read(int fd, char *buf, uint64_t count) {
    return syscall_read(fd, buf, count);
}

int64_t get_time(rtc_time_t *time_ptr) {
    return syscall_get_time(time_ptr);
}

int64_t get_datetime(rtc_datetime_t *datetime_ptr) {
    return syscall_get_datetime(datetime_ptr);
}