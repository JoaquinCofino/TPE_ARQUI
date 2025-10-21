#include <stdint.h>
#include "../../include/ejercicios.h"

extern uint8_t read_port(uint16_t num);
void int_20();
void process_keyboard();

void int_20(){
    timer_handler();
}

#define SCANCODE_BUFFER_SIZE 32
uint8_t scancodeBuffer[SCANCODE_BUFFER_SIZE];
volatile uint8_t bufferHead = 0;
volatile uint8_t bufferTail = 0;

void keyboard_handler() {
    uint8_t scancode = read_port(0x60);

    // Guardar en buffer circular
    uint8_t nextHead = (bufferHead + 1) % SCANCODE_BUFFER_SIZE;
    if (nextHead != bufferTail) { // evitar sobreescribir
        scancodeBuffer[bufferHead] = scancode;
        bufferHead = nextHead;
    }
}

void process_keyboard() {                        //se llama dentro del bucle en el kernel
    while (bufferTail != bufferHead) {           //obs : sin el if se imprimen dos códigos (make y break) al apretar una sola tecla por cómo funcionan los teclados
        uint8_t sc = scancodeBuffer[bufferTail];
        bufferTail = (bufferTail + 1) % SCANCODE_BUFFER_SIZE;

        //si el bit más alto está en 1, no imprime nada, es un break code.
        if (sc & 0x80) return;

        // Convertir a hexadecimal para mostrar
        char hex[3];
        hex[0] = "0123456789ABCDEF"[(sc >> 4) & 0xF];
        hex[1] = "0123456789ABCDEF"[sc & 0xF];
        hex[2] = '\0';

        ej1("Key pressed! Scancode: ");
        ej1(hex);
        ej1("\n");
    }
}

void irq_default() {
    // No hacer nada
}

#define NUM_IRQS 256
void (*irqHandlers[NUM_IRQS])() = {irq_default}; // Inicializamos todas a irq_default


void initIRQHandlers() {
    irqHandlers[0] = int_20;                    // IRQ0 = timer
    irqHandlers[1] = keyboard_handler;          //IRQ1 = keyboard (ej6)
    // para agregar más handlers:
    // irqHandlers[2] = otro_handler;
}

void irqDispatcher(uint64_t irq) {
    if (irq < NUM_IRQS && irqHandlers[irq]) irqHandlers[irq]();  // Llamada al handler correspondiente
}

/*void irqDispatcher(uint64_t irq){
    switch (irq)
    {
    case 0: int_20(); break;
    }
}feo!*/