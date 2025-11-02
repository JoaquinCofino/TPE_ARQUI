#ifndef IO_PORTS_H
#define IO_PORTS_H

#include <stdint.h>

// Escribe un byte al puerto especificado
void outb(uint16_t port, uint8_t value);

// Lee un byte desde el puerto especificado
uint8_t inb(uint16_t port);

#endif