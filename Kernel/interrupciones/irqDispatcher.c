#include <stdint.h>
#include <naiveConsole.h>

extern uint8_t read_port(uint16_t num);
void int_20();
extern void keyboard_handler();
extern void timer_handler();
void int_20(){
    timer_handler();
}

void irq_default() {
    // No hacer nada
}

#define NUM_IRQS 256
void (*irqHandlers[NUM_IRQS])() = {irq_default}; // Inicializamos todas a irq_default


void initIRQHandlers() {
    irqHandlers[0] = int_20;                    // IRQ0 = timer
    irqHandlers[1] = keyboard_handler;          //IRQ1 = keyboard 
    // para agregar más handlers:
    // irqHandlers[2] = otro_handler;
}

void irqDispatcher(uint64_t irq) {
    if (irq < NUM_IRQS && irqHandlers[irq]) irqHandlers[irq]();  // Llamada al handler correspondiente
}