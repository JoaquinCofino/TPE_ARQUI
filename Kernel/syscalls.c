#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#define SYS_WRITE 1
uint64_t syscall_delegator(uint64_t syscall_num, uint64_t arg1, uint64_t arg2, uint64_t arg3){
    switch (syscall_num) {
        case SYS_WRITE:
            return sys_write(arg1, (const char *)arg2, (uint8_t)arg3);
        default:
            return -1;
    }
}

int64_t sys_write(int fd, const char *buf, uint64_t count){
    if (fd == 1) {  // stdout
        for (uint8_t i = 0; i < count; i++)
            ncPrint(buf[i]);  // o tu propia función de video
        return count;
    }
    return -1;
}