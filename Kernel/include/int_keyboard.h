#ifndef INT_KEYBOARD_H
#define INT_KEYBOARD_H

#include <stdint.h>

// Scancodes de teclas modificadoras
#define LSHIFT_SCANCODE 0x2A
#define RSHIFT_SCANCODE 0x36
#define LALT_SCANCODE   0x38
#define RALT_SCANCODE   0xB8
#define CAPS_SCANCODE   0x3A

// Tamaño del buffer de scancodes
#define SCANCODE_BUFFER_SIZE 32

// Funciones principales del driver de teclado
void keyboard_handler(void);
void process_keyboard(void);

// Función auxiliar para leer puertos
extern uint8_t read_port(uint16_t num);
extern void write_port(uint16_t port, uint8_t value);

#endif 