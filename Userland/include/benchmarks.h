#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

// Prototipos de funciones de benchmark
void benchmark_cpu(void);    // Benchmark de CPU (operaciones int/float)
void benchmark_mem(void);    // Benchmark de memoria (escritura/lectura/copia)
void benchmark_fps(void);    // Benchmark de FPS (limpieza de pantalla)
void benchmark_key(void);    // Benchmark de tiempo de respuesta de teclado

void benchmark_run_all(void);

#endif // BENCHMARK_H