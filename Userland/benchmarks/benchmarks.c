#include "benchmarks.h"
#include "stdio.h"
#include "syscalls.h"

#define KB 1024
#define BM_BUFF 32

static void print_benchmark_number(const char* label, uint64_t value);
static void print_operations_per_tick(uint64_t operations, uint64_t delta);

void benchmark_cpu(void) {
    /*
     * benchmark_cpu - benchmark simple de CPU
     * - Ejecuta una mezcla de operaciones enteras y de punto flotante N veces
     * - Mide ticks transcurridos usando get_ticks() e imprime el tiempo total
     *   y una métrica aproximada de operaciones por tick.
     */
    clear_screen();
    puts("\n=== CPU Benchmark ===");
    puts("Ejecutando operaciones int/float...");
    
    const uint32_t N = 1000000u;
    uint64_t start_ticks = get_ticks();

    uint64_t result = 0;
    double float_result = 0.0;

    for(uint32_t i = 0; i < N; ++i) {
        result += (uint64_t)i * 7ull;
        result = result % 2147483647ull;

        float_result += (double)i * 3.14159;
        if ((i % 100000u) == 0 && i != 0) {
            float_result *= 0.5;
        }
    }

    uint64_t end_ticks = get_ticks();
    uint64_t delta = end_ticks - start_ticks;

    print_benchmark_number("Tiempo", delta);

    if(delta > 0) {
        uint64_t ops_per_tick = ((uint64_t)N) / delta;
        print_benchmark_number("Operaciones por tick", ops_per_tick);
    } else {
        puts("Tiempo transcurrido = 0, no se puede calcular ops/tick.");
    }

    if(result == 0 && float_result == 0.0) {
        puts("(resultados procesados correctamente)");
    }
    
    puts("\nPresione una tecla para continuar...");
    getchar();
}

void benchmark_fps(void) {    
    /*
     * benchmark_fps - benchmark de operaciones de video
     * - Dibuja múltiples cuadrados en pantalla y mide el tiempo
     * - Calcula FPS reales basados en frames dibujados
     * - Muestra resultados ABAJO de los cuadrados
     */
    clear_screen();
    
    puts("=== Video Benchmark ===");
    puts("Iniciando en 2 segundos...");
    puts("");
    
    // Pequeña pausa antes de empezar
    uint64_t pause_start = get_ticks();
    while((get_ticks() - pause_start) < 36); 
    
    clear_screen();

    uint64_t start_ticks = get_ticks();
    uint64_t frames = 0;
    uint64_t duration = 18 * 3; 

    const int square_size = 50;
    const int spacing = 10;
    
    const int draw_area_height = 350;
    int x = 10;
    int y = 10;
    
    // Colores disponibles
    uint32_t colors[] = {
        0xFF0000, // Rojo
        0x00FF00, // Verde
        0x0000FF, // Azul
        0xFFFF00, // Amarillo
        0xFF00FF, // Magenta
        0x00FFFF  // Cyan
    };
    int color_index = 0;

    while((get_ticks() - start_ticks) < duration) {
        video_draw_rect(x, y, square_size, square_size, colors[color_index]);
        
        frames++;
        
        x += square_size + spacing;
        if(x + square_size > 700) {
            x = 10;
            y += square_size + spacing;
            if(y + square_size > draw_area_height) {
                y = 10;
            }
        }
        
        color_index = (color_index + 1) % 6;
    }

    uint64_t end_ticks = get_ticks();
    uint64_t actual_ticks = end_ticks - start_ticks;

    uint64_t fps = 0;
    uint64_t milliseconds = 0;
    
    if(actual_ticks > 0) {
        fps = (frames * 18) / actual_ticks;
        milliseconds = (actual_ticks * 1000) / 18;
    }
    set_cursor_position(10, draw_area_height + 30);
    
    video_clear();

    puts("=== Resultados Video Benchmark ===");
    printf("Dibujos hechos: ");
    puts_number(frames);
    putchar('\n');
    
    printf("Tiempo: ");
    puts_number(actual_ticks);
    puts(" ticks");
    
    printf("Tiempo: ");
    puts_number(milliseconds);
    puts(" ms");
    
    printf("FPS (Frames por segundo): ");
    puts_number(fps);
    putchar('\n');
    
    puts("\nPresione una tecla para continuar...");
    getchar();
}

void benchmark_mem(void) {
    /*
     * benchmark_mem - benchmark simple de memoria
     * - Llena un buffer de 4KB muchas veces, calcula checksum y hace copias.
     * - Mide ticks transcurridos y reporta operaciones por tick.
     */
    clear_screen();
    puts("\n=== Memory Benchmark ===");
    puts("Ejecutando operaciones de memoria...");
    
    char buffer[4 * KB];
    uint64_t start_ticks = get_ticks();

    const int iterations = 10000;
    
    for(int iteration = 0; iteration < iterations; iteration++) {
        for(int i = 0; i < 4 * KB; i++) {
            buffer[i] = (i + iteration) % 256;
        }

        volatile uint64_t checksum = 0;
        for(int i = 0; i < 4 * KB; i++) {
            checksum += (unsigned char)buffer[i];
            checksum = checksum % 1000000ULL;
        }

        for(int i = 0; i < 2 * KB; i++) {
            buffer[i + 2 * KB] = buffer[i];
        }
    }

    uint64_t end_ticks = get_ticks();
    uint64_t delta = end_ticks - start_ticks;

    print_benchmark_number("Tiempo", delta);

    if(delta > 0) {
        uint64_t operations = (uint64_t)iterations * (uint64_t)(4 * KB) * 3ULL;
        print_operations_per_tick(operations, delta);
    }
    
    puts("\nPresione una tecla para continuar...");
    getchar();
}

void benchmark_key(void) {
    clear_screen();
    puts("\n=== Key Response Benchmark ===");
    puts("Presione cualquier tecla cuando este listo...");
    
    uint64_t start_ticks = get_ticks();
    
    getchar();
    
    uint64_t end_ticks = get_ticks();
    uint64_t delta = end_ticks - start_ticks;
    
    print_benchmark_number("Tiempo de respuesta", delta);
    
    if(delta > 0) {
        uint64_t milliseconds = (delta * 1000) / 18;
        print_benchmark_number("Milisegundos aprox", milliseconds);
    }
    
    puts("\nPresione una tecla para continuar...");
    getchar();
}

// Ejecuta todos los benchmarks
void benchmark_run_all(void) {
    puts("\n========================================");
    puts("    SISTEMA DE BENCHMARKS");
    puts("========================================\n");
    
    benchmark_cpu();
    
    benchmark_mem();
    
    benchmark_fps();
    

    
    puts("\n========================================");
    puts("    BENCHMARKS COMPLETADOS");
    puts("========================================\n");
}

static void print_benchmark_number(const char* label, uint64_t value) {
    printf(label);
    printf(": ");
    puts_number(value);
    puts(" ticks");
}

static void print_operations_per_tick(uint64_t operations, uint64_t delta) {
    uint64_t ops_per_tick = operations / delta;
    printf("Operaciones por tick: ");
    puts_number(ops_per_tick);
    putchar('\n');
}