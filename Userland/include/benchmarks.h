#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../include/syscalls.h"
#include <stdint.h>

#define true 1
#define false 0
typedef uint8_t bool;

typedef struct {
    // Contadores básicos
    uint64_t total_frames;
    uint64_t frames_this_second;
    
    // Tiempos (en frames, no en ms)
    uint64_t processing_frames;
    uint64_t floating_point_frames;
    uint64_t hardware_frames;
    uint64_t render_frames;
    
    // Estadísticas FPS
    uint32_t current_fps;
    uint32_t average_fps;
    uint32_t min_fps;
    uint32_t max_fps;
    
    // Estado
    bool is_active;
    uint64_t last_second_frame;
    
} BenchmarkData;

// Inicialización y control
void benchmark_init(void);
void benchmark_start(void);
void benchmark_stop(void);
void benchmark_reset(void);

// Mediciones de frame
void benchmark_frame_start(void);
void benchmark_frame_end(void);

// Mediciones específicas (contamos frames, no tiempo)
void benchmark_processing_start(void);
void benchmark_processing_end(void);
void benchmark_floating_point_start(void);
void benchmark_floating_point_end(void);
void benchmark_hardware_start(void);
void benchmark_hardware_end(void);
void benchmark_render_start(void);
void benchmark_render_end(void);

// Obtención de resultados
uint32_t benchmark_get_current_fps(void);
uint32_t benchmark_get_average_fps(void);
BenchmarkData* benchmark_get_data(void);

// Reportes
void benchmark_print_status(void);
void benchmark_print_report(void);

#endif