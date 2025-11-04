#ifndef STDIO_H
#define STDIO_H

#include "syscalls.h"

void putchar(char c);
void puts(const char *s);
void printf(const char *s);
int getchar(void);
void print_number(unsigned int num);  // CAMBIADO de puts_number
void print_date(void);

// Nuevas funciones útiles
void print_registers(void);
void print_video_info(void);
void print_hex(uint64_t num);
void clear_screen(void);
int scanf(char *buffer);
int getchar_nb(void);

#endif