#include <stdint.h>
#include <naiveConsole.h>
#include "interrupts.h"

extern uint8_t read_port(uint16_t num);
extern void keyboard_handler();
extern void process_keyboard();  // ← Agregar declaración
extern void timer_handler();

void int_20(){
    timer_handler();
}

void int_21() {  // ← Crear nuevo handler para IRQ1
    keyboard_handler();   // Lee del puerto 0x60
    process_keyboard();   // Procesa y convierte a ASCII
}

void irq_default() {
    // No hacer nada
}

#define NUM_IRQS 256
void (*irqHandlers[NUM_IRQS])() = {irq_default};

void initIRQHandlers() {
    irqHandlers[0] = int_20;   // IRQ0 = timer
    irqHandlers[1] = int_21;   // IRQ1 = keyboard (ahora con wrapper)
}

void irqDispatcher(uint64_t irq) {
    if (irq < NUM_IRQS && irqHandlers[irq]) {
        irqHandlers[irq]();
    }
}