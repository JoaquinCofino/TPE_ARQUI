#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void sleep_ms(uint32_t ms);

char *cpuVendor(char *result);

extern uint8_t read_port(uint16_t num);
extern void write_port(uint16_t port, uint8_t value);

#endif