
#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include "idtLoader.h"

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _exception0Handler(void);
void _exception6Handler(void);

// Dispatcher de excepciones
void exceptionDispatcher(uint64_t exception, uint64_t rip);

// Inicialización de handlers de IRQ
void initIRQHandlers(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

#endif 
