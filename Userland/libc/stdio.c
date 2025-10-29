#include "stdio.h"
#include "syscalls.h"

void putchar(char c) {
    write(1, &c, 1);
}

void puts(const char *s) {
    int len = 0;
    while (s[len]) len++;
    write(1, s, len);
}

int getchar(void) {
    char c;
    if (read(0, &c, 1) == 1) {
        return c;
    }
    return -1;
}