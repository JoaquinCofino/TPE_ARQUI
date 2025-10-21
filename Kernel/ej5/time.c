#include "../../include/ejercicios.h"
void timer_handler();
static unsigned long ticks = 0;
void timer_handler(){
    ticks++;

    // cada ~5 segundos
    if (ticks % 100 == 0) ej1("Tick!\n");
}
