#include "../include/syscall.h"
#include "../include/screens.h"
#include "../include/stdio.h"

#include <stdint.h>

#define TICK_DELAY 800000  // ajusta velocidad

typedef struct {
    int x, y;
    char dir;     // 'U','D','L','R'
    int alive;
    uint32_t color;
} Player;

static Player p1, p2;

static void wait_tick(void) {
    for (volatile int i = 0; i < TICK_DELAY; i++);
}

// Drenar teclas pendientes (evita salir inmediatamente)
static inline void kbd_drain(void) {
    while (getchar_nb() >= 0) { /* discard */ }
}

// Borde con draw_rect para evitar dudas con putpixel
static void draw_border(void) {
    video_info_t v; get_video_data(&v);
    uint16_t w = v.width, h = v.height;
    uint32_t c = 0xFFFFFF;
    video_draw_rect(0, 0, w, 1, c);           // top
    video_draw_rect(0, h-1, w, 1, c);         // bottom
    video_draw_rect(0, 0, 1, h, c);           // left
    video_draw_rect(w-1, 0, 1, h, c);         // right
}

// Cabeza 3x3 usando draw_rect
static inline void draw_head(Player *p) {
    video_draw_rect(p->x - 1, p->y - 1, 3, 3, p->color);
}

// Cola 1x1 en la posición anterior
static inline void draw_trail(int x, int y, uint32_t color) {
    video_draw_rect(x, y, 1, 1, color);
}

static void move_player(Player *p) {
    int old_x = p->x, old_y = p->y;
    switch (p->dir) {
        case 'U': p->y--; break;
        case 'D': p->y++; break;
        case 'L': p->x--; break;
        case 'R': p->x++; break;
    }
    // Deja cola en el píxel central anterior
    draw_trail(old_x, old_y, p->color);
}

static int check_border_collision(Player *p, uint16_t w, uint16_t h) {
    // margen por cabeza 3x3
    return (p->x <= 1 || p->x >= (int)w - 2 || p->y <= 1 || p->y >= (int)h - 2);
}

static int check_player_collision(Player *a, Player *b) {
    // AABB 3x3
    if (a->x + 1 < b->x - 1 || a->x - 1 > b->x + 1) return 0;
    if (a->y + 1 < b->y - 1 || a->y - 1 > b->y + 1) return 0;
    return 1;
}

static uint32_t rnd_state = 0;

static void rnd_seed(uint32_t seed) {
    rnd_state = seed;
}

static int rnd(void) { 
    rnd_state = rnd_state * 1664525u + 1013904223u; 
    return (int)(rnd_state >> 16); 
}

static void cpu_move(Player *cpu) {
    video_info_t v; get_video_data(&v);
    uint16_t W = v.width, H = v.height;
    // Margen de seguridad para evitar bordes
    int safe_margin = 50;
    
    // Prioridad 1: Evitar bordes cercanos (comportamiento defensivo)
    if (cpu->x < safe_margin && cpu->dir == 'L') {
        cpu->dir = (rnd() & 1) ? 'U' : 'D';  // Gira arriba o abajo
    }
    else if (cpu->x > W - safe_margin && cpu->dir == 'R') {
        cpu->dir = (rnd() & 1) ? 'U' : 'D';
    }
    else if (cpu->y < safe_margin && cpu->dir == 'U') {
        cpu->dir = (rnd() & 1) ? 'L' : 'R';  // Gira izquierda o derecha
    }
    else if (cpu->y > H - safe_margin && cpu->dir == 'D') {
        cpu->dir = (rnd() & 1) ? 'L' : 'R';
    }
    // Prioridad 2: Cambiar dirección ocasionalmente (5% de probabilidad)
    // Esto simula decisiones "tácticas" humanas
    else if ((rnd() % 100) < 5) {
        int m = rnd() & 3;
        if (m == 0 && cpu->dir != 'D') cpu->dir = 'U';
        else if (m == 1 && cpu->dir != 'U') cpu->dir = 'D';
        else if (m == 2 && cpu->dir != 'R') cpu->dir = 'L';
        else if (m == 3 && cpu->dir != 'L') cpu->dir = 'R';
    }
}

static int select_mode(void) {
    // Mostrar pantalla gráfica de modo
    mode_screen();
    

    int option = 0;
    while (option == 0) {
        int c = getchar();
        if (c == '1') option = 1;
        else if (c == '2') option = 2;
        else if (c == 'q' || c == 'Q') option = 3;
    }

    clear_screen();
    draw_border();
    kbd_drain(); // limpia buffer para evitar arrastre de teclas
    return option;
}

void tron_main(void) {
    video_info_t v; get_video_data(&v);
    uint16_t W = v.width, H = v.height;

    clear_screen();
    draw_border();

    int mode = select_mode();
    

    // Init jugadores
    p1 = (Player){ .x = W/4,     .y = H/2, .dir = 'R', .alive = 1, .color = 0xFF0000 };
    p2 = (Player){ .x = (3*W)/4, .y = H/2, .dir = 'L', .alive = 1, .color = 0x0000FF };

    draw_head(&p1);
    draw_head(&p2);

    while (p1.alive && p2.alive) {
        // Leer tecla no bloqueante y actualizar dirección
        int k = getchar_nb();
        if (k >= 0) {
            if ((k=='w'||k=='W') && p1.dir!='D') p1.dir='U';
            else if ((k=='s'||k=='S') && p1.dir!='U') p1.dir='D';
            else if ((k=='a'||k=='A') && p1.dir!='R') p1.dir='L';
            else if ((k=='d'||k=='D') && p1.dir!='L') p1.dir='R';
            else if (k=='q'||k=='Q') { p1.alive=0; p2.alive=0; }
        }

        // Jugador 2 (solo si es multiplayer)
            if (mode == 2) {
                if ((k=='i'||k=='I') && p2.dir!='D') p2.dir='U';
                else if ((k=='k'||k=='K') && p2.dir!='U') p2.dir='D';
                else if ((k=='j'||k=='J') && p2.dir!='R') p2.dir='L';
                else if ((k=='l'||k=='L') && p2.dir!='L') p2.dir='R';
            }

        if (mode == 3) { return;}
        

        move_player(&p1);

        if (mode == 1) {
            cpu_move(&p2);      // Cambia dirección aleatoriamente
            move_player(&p2);   // AHORA SÍ SE MUEVE
        } else {
            move_player(&p2);   // Modo PvP
}

        // Colisiones
        if (check_border_collision(&p1, W, H)) p1.alive = 0;
        if (check_border_collision(&p2, W, H)) p2.alive = 0;
        if (p1.alive && p2.alive && check_player_collision(&p1, &p2)) { p1.alive=0; p2.alive=0; }

        // Dibujar cabezas (si siguen vivos)
        if (p1.alive) draw_head(&p1);
        if (p2.alive) draw_head(&p2);

        wait_tick();
    }

    putchar('\n');
    if (p1.alive && !p2.alive) puts("Gana Jugador 1 (Rojo)");
    else if (!p1.alive && p2.alive) puts("Gana CPU (Azul)");
    else puts("Empate");

    puts("Presiona cualquier tecla para volver...");
    getchar();         // <-- espera una nueva tecla (bloqueante)
}
