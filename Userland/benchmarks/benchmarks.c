#include "../include/benchmarks.h"
#include "../include/stdio.h"
#include <stdint.h>

BenchmarkData bench_data = {0};
static bool measuring_processing = false;
static bool measuring_floating_point = false;
static bool measuring_hardware = false;
static bool measuring_render = false;

void benchmark_init(void) {
    bench_data.is_active = false;
    bench_data.total_frames = 0;
    bench_data.frames_this_second = 0;
    bench_data.current_fps = 0;
    bench_data.average_fps = 0;
    bench_data.min_fps = UINT32_MAX;
    bench_data.max_fps = 0;
    
    bench_data.processing_frames = 0;
    bench_data.floating_point_frames = 0;
    bench_data.hardware_frames = 0;
    bench_data.render_frames = 0;
    
    bench_data.last_second_frame = 0;
}

void benchmark_start(void) {
    benchmark_init();
    bench_data.is_active = true;
}

void benchmark_stop(void) {
    bench_data.is_active = false;
}

void benchmark_reset(void) {
    benchmark_init();
}

void benchmark_frame_start(void) {
    if (!bench_data.is_active) return;
    // Podemos usar esto para medir tiempo real si conseguimos mejor precisión
}

void benchmark_frame_end(void) {
    if (!bench_data.is_active) return;
    
    bench_data.total_frames++;
    bench_data.frames_this_second++;
    
    // Calcular FPS cada frame (aproximado)
    bench_data.current_fps = bench_data.frames_this_second;
    
    // Actualizar min/max
    if (bench_data.current_fps < bench_data.min_fps) {
        bench_data.min_fps = bench_data.current_fps;
    }
    if (bench_data.current_fps > bench_data.max_fps) {
        bench_data.max_fps = bench_data.current_fps;
    }
    
    // Resetear contador cada "segundo" (cada 60 frames aprox)
    if (bench_data.total_frames - bench_data.last_second_frame >= 60) {
        bench_data.average_fps = bench_data.frames_this_second;
        bench_data.frames_this_second = 0;
        bench_data.last_second_frame = bench_data.total_frames;
    }
}

// Contamos en qué frames se ejecuta cada tipo de operación
void benchmark_processing_start(void) {
    if (!bench_data.is_active) return;
    measuring_processing = true;
}

void benchmark_processing_end(void) {
    if (!bench_data.is_active) return;
    if (measuring_processing) {
        bench_data.processing_frames++;
        measuring_processing = false;
    }
}

void benchmark_floating_point_start(void) {
    if (!bench_data.is_active) return;
    measuring_floating_point = true;
}

void benchmark_floating_point_end(void) {
    if (!bench_data.is_active) return;
    if (measuring_floating_point) {
        bench_data.floating_point_frames++;
        measuring_floating_point = false;
    }
}

void benchmark_hardware_start(void) {
    if (!bench_data.is_active) return;
    measuring_hardware = true;
}

void benchmark_hardware_end(void) {
    if (!bench_data.is_active) return;
    if (measuring_hardware) {
        bench_data.hardware_frames++;
        measuring_hardware = false;
    }
}

void benchmark_render_start(void) {
    if (!bench_data.is_active) return;
    measuring_render = true;
}

void benchmark_render_end(void) {
    if (!bench_data.is_active) return;
    if (measuring_render) {
        bench_data.render_frames++;
        measuring_render = false;
    }
}

uint32_t benchmark_get_current_fps(void) {
    return bench_data.current_fps;
}

uint32_t benchmark_get_average_fps(void) {
    return bench_data.average_fps;
}

BenchmarkData* benchmark_get_data(void) {
    return &bench_data;
}

void benchmark_print_status(void) {
    printf("FPS: ");
    puts_number(bench_data.current_fps);
    printf("\n");
}

void benchmark_print_report(void) {
    puts("=== BENCHMARK REPORT ===");
    
    printf("Frames totales: ");
    puts_number(bench_data.total_frames);
    printf("\n");
    
    printf("FPS actual: ");
    puts_number(bench_data.current_fps);
    printf("\n");
    
    printf("FPS promedio: ");
    puts_number(bench_data.average_fps);
    printf("\n");
    
    printf("FPS minimo: ");
    puts_number(bench_data.min_fps);
    printf("\n");
    
    printf("FPS maximo: ");
    puts_number(bench_data.max_fps);
    printf("\n");
    
    puts("--- Frames por categoria ---");
    printf("Procesamiento: ");
    puts_number(bench_data.processing_frames);
    printf("\n");
    
    printf("Floating Point: ");
    puts_number(bench_data.floating_point_frames);
    printf("\n");
    
    printf("Hardware: ");
    puts_number(bench_data.hardware_frames);
    printf("\n");
    
    printf("Render: ");
    puts_number(bench_data.render_frames);
    printf("\n");
    
    // Porcentajes
    if (bench_data.total_frames > 0) {
        puts("--- Porcentajes ---");
        printf("Procesamiento: ");
        puts_number((bench_data.processing_frames * 100) / bench_data.total_frames);
        printf("%%\n");
        
        printf("Floating Point: ");
        puts_number((bench_data.floating_point_frames * 100) / bench_data.total_frames);
        printf("%%\n");
        
        printf("Hardware: ");
        puts_number((bench_data.hardware_frames * 100) / bench_data.total_frames);
        printf("%%\n");
        
        printf("Render: ");
        puts_number((bench_data.render_frames * 100) / bench_data.total_frames);
        printf("%%\n");
    }
}