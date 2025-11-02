#include "../../Kernel/include/videoDriver.h"  // Para putPixel
#include "../include/stdio.h"

typedef struct {
    int x, y;
    char dir;     // 'U', 'D', 'L', 'R'
    int alive;
    uint32_t color;
} Player;

static Player p1, p2;

void draw_border() {
    uint16_t width = getScreenWidth();
    uint16_t height = getScreenHeight();
    
    // Dibujar bordes
    for (int x = 0; x < width; x++) {
        putPixel(0xFFFFFF, x, 0);                    // Borde superior
        putPixel(0xFFFFFF, x, height - 1);           // Borde inferior
    }
    for (int y = 0; y < height; y++) {
        putPixel(0xFFFFFF, 0, y);                    // Borde izquierdo
        putPixel(0xFFFFFF, width - 1, y);            // Borde derecho
    }
}

void draw_player(Player *p) {
    // Dibujar un cuadrado 3x3 para el jugador
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            putPixel(p->color, p->x + dx, p->y + dy);
        }
    }
}

void clear_player(Player *p) {
    // Limpiar la posición anterior del jugador (negro)
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            putPixel(0x000000, p->x + dx, p->y + dy);
        }
    }
}

void move_player(Player *p) {
    // Guardar posición anterior para limpiar
    int old_x = p->x;
    int old_y = p->y;
    
    // Mover
    switch (p->dir) {
        case 'U': p->y--; break;
        case 'D': p->y++; break;
        case 'L': p->x--; break;
        case 'R': p->x++; break;
    }
    
    // Limpiar posición anterior y dejar estela
    putPixel(p->color, old_x, old_y);  // Dejar estela del color del jugador
}

int check_collision(Player *p, uint16_t width, uint16_t height) {
    // Choca con los bordes (con margen para el tamaño del jugador)
    if (p->x <= 2 || p->x >= width - 3 || p->y <= 2 || p->y >= height - 3)
        return 1;
    
    // Verificar si la nueva posición ya tiene color (colisión con estela)
    // Esto es simplificado - en una versión real necesitarías leer el pixel
    return 0;
}

// Función para leer el color de un pixel (necesitarías implementarla)
uint32_t getPixel(int x, int y) {
    // Esta función depende de tu implementación del framebuffer
    // Por ahora retornamos 0 (negro) como placeholder
    return 0x000000;
}

int simple_rand() {
    static unsigned int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (int)seed;
}

void cpu_move(Player *cpu) {
    // Movimiento CPU: aleatorio pero evita dirección opuesta
    int move = simple_rand() % 4;
    switch (move) {
        case 0: if (cpu->dir != 'D') cpu->dir = 'U'; break;
        case 1: if (cpu->dir != 'U') cpu->dir = 'D'; break;
        case 2: if (cpu->dir != 'R') cpu->dir = 'L'; break;
        case 3: if (cpu->dir != 'L') cpu->dir = 'R'; break;
    }
}

void tron_main() {
    uint16_t width = getScreenWidth();
    uint16_t height = getScreenHeight();
    
    clear_screen();
    
    // Inicializar jugadores
    p1.x = width / 4;
    p1.y = height / 2;
    p1.dir = 'R';
    p1.color = 0xFF0000;  // Rojo
    p1.alive = 1;
    
    p2.x = (width * 3) / 4;
    p2.y = height / 2;
    p2.dir = 'L';
    p2.color = 0x0000FF;  // Azul
    p2.alive = 1;
    
    draw_border();
    draw_player(&p1);
    draw_player(&p2);

    while (p1.alive && p2.alive) {
        int key = getchar();
        
        // Procesar teclas
        if (key == 'w' && p1.dir != 'D') p1.dir = 'U';
        else if (key == 's' && p1.dir != 'U') p1.dir = 'D';
        else if (key == 'a' && p1.dir != 'R') p1.dir = 'L';
        else if (key == 'd' && p1.dir != 'L') p1.dir = 'R';
        else if (key == 'q') break;

        // Limpiar jugadores de sus posiciones actuales
        clear_player(&p1);
        clear_player(&p2);

        // Mover
        move_player(&p1);
        cpu_move(&p2);
        move_player(&p2);

        // Chequear colisiones
        if (check_collision(&p1, width, height)) p1.alive = 0;
        if (check_collision(&p2, width, height)) p2.alive = 0;

        // Dibujar jugadores en nuevas posiciones
        if (p1.alive) draw_player(&p1);
        if (p2.alive) draw_player(&p2);
        
        // Pequeña pausa
        for (volatile int i = 0; i < 1000000; i++);
    }

    // Mostrar resultado (usando printf de texto)
    printf("\n\n");  // Bajar unas líneas
    if (p1.alive && !p2.alive)
        printf("¡Jugador 1 gana! (Rojo)");
    else if (!p1.alive && p2.alive)
        printf("¡La CPU gana! (Azul)");
    else
        printf("Empate.");
        
    printf("\nPresiona cualquier tecla para continuar...");
    getchar();
}

void tron_command() {
    tron_main();
}