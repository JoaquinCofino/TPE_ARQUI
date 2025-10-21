#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>

uint32_t syscall_dispatcher(uint32_t syscall_num, uint32_t arg1, uint32_t arg2, uint32_t arg3){
    switch (syscall_num) {
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, (uint8_t)arg3);
        default:
            return -1;
    }
}

int sys_write(int fd, const char *buf, uint8_t count) {
    if (fd == 1) {  // stdout
        for (uint8_t i = 0; i < count; i++)
            putchar(buf[i]);  // o tu propia función de video
        return count;
    }
    return -1;
}