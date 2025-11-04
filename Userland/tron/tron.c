#include "../include/syscall.h"
#include "../include/screens.h"
#include "../include/stdio.h"
#include "tron.h"

#include <stdint.h>

static Player p1, p2;
int current_level = 0;

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

static int check_collision(int x, int y, int direction, uint32_t color1, uint32_t color2) {
    // Calcular la posición adelante según la dirección
    int check_x = x, check_y = y;
    
    switch (direction) {
        case 'U':    check_y = y - 1; break;  // Arriba
        case 'D':    check_y = y + 1; break;  // Abajo
        case 'L':    check_x = x - 1; break;  // Izquierda
        case 'R':    check_x = x + 1; break;  // Derecha
    }
    
    uint32_t pixel_color = video_getpixel(check_x, check_y);
    return (pixel_color == color1 || pixel_color == color2);  // Chequea ambos colores
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

// --- Función auxiliar: chequea si hay espacio libre adelante ---
    int is_safe(int x, int y) {
         video_info_t v; 
    get_video_data(&v);
    uint16_t W = v.width, H = v.height;

        if (x <= 1 || x >= (int)W - 2 || y <= 1 || y >= (int)H - 2)
            return 0;
        uint32_t color = video_getpixel(x, y);
        return (color == 0x000000);
    }

static void cpu_move(Player *cpu) {
    video_info_t v; 
    get_video_data(&v);
    uint16_t W = v.width, H = v.height;

    //const int lookahead = 20;    // mira varios píxeles adelante
    //const int turn_chance = 1;  // baja chance de girar "porque sí"

    // MIRA MÁS LEJOS EN NIVELES ALTOS
    int lookahead = 20 + (current_level * 2);  // +2 píxeles por nivel
    if (lookahead > 40) lookahead = 40;  // máximo 40 píxeles
    
    // MÁS PROBABILIDAD DE GIROS TÁCTICOS EN NIVELES ALTOS
    int turn_chance = 1 + (current_level / 2);  // +0.5% por nivel
    
    // REACCIÓN MÁS RÁPIDA
    int reaction_delay = 5 - (current_level / 2);
    if (reaction_delay < 1) reaction_delay = 1;

    static int frame_counter = 0;
    frame_counter++;
    if (frame_counter % reaction_delay != 0)
        return;  // se mueve solo cada algunos frames

    // --- Calcular puntos a verificar ---
    int fx = cpu->x, fy = cpu->y;
    int lx = cpu->x, ly = cpu->y;
    int rx = cpu->x, ry = cpu->y;

    switch (cpu->dir) {
        case 'U':
            fy -= lookahead; lx -= lookahead; rx += lookahead;
            break;
        case 'D':
            fy += lookahead; lx += lookahead; rx -= lookahead;
            break;
        case 'L':
            fx -= lookahead; ly += lookahead; ry -= lookahead;
            break;
        case 'R':
            fx += lookahead; ly -= lookahead; ry += lookahead;
            break;
    }

    int forward_safe = is_safe(fx, fy);
    int left_safe = is_safe(lx, ly);
    int right_safe = is_safe(rx, ry);

    // --- Decisión de dirección ---
    if (!forward_safe) {
        // Si hay peligro al frente → elegir lado libre
        if (left_safe && !right_safe) {
            cpu->dir = (cpu->dir == 'U') ? 'L' :
                       (cpu->dir == 'D') ? 'R' :
                       (cpu->dir == 'L') ? 'D' : 'U';
        } else if (right_safe && !left_safe) {
            cpu->dir = (cpu->dir == 'U') ? 'R' :
                       (cpu->dir == 'D') ? 'L' :
                       (cpu->dir == 'L') ? 'U' : 'D';
        } else if (left_safe && right_safe) {
            if (rnd() & 1)
                cpu->dir = (cpu->dir == 'U') ? 'L' :
                           (cpu->dir == 'D') ? 'R' :
                           (cpu->dir == 'L') ? 'D' : 'U';
            else
                cpu->dir = (cpu->dir == 'U') ? 'R' :
                           (cpu->dir == 'D') ? 'L' :
                           (cpu->dir == 'L') ? 'U' : 'D';
        } else {
            // sin salida: último recurso → giro aleatorio
            int m = rnd() & 3;
            if (m == 0 && cpu->dir != 'D') cpu->dir = 'U';
            else if (m == 1 && cpu->dir != 'U') cpu->dir = 'D';
            else if (m == 2 && cpu->dir != 'R') cpu->dir = 'L';
            else if (m == 3 && cpu->dir != 'L') cpu->dir = 'R';
        }
    } else if ((rnd() % 100) < turn_chance) {
        // chance baja de girar por decisión táctica
        if (left_safe && (rnd() & 1)) {
            cpu->dir = (cpu->dir == 'U') ? 'L' :
                       (cpu->dir == 'D') ? 'R' :
                       (cpu->dir == 'L') ? 'D' : 'U';
        } else if (right_safe) {
            cpu->dir = (cpu->dir == 'U') ? 'R' :
                       (cpu->dir == 'D') ? 'L' :
                       (cpu->dir == 'L') ? 'U' : 'D';
        }
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

void init_player(Player *p1, Player *p2) {
    video_info_t v; 
    get_video_data(&v);
    uint16_t W = v.width, H = v.height;
    p1->x = W/4;     p1->y = H/2; p1->dir = 'R'; p1->alive = 1; p1->color = 0xFF0000;
    p2->x = (3*W)/4; p2->y = H/2; p2->dir = 'L'; p2->alive = 1; p2->color = 0x0000FF;
}

char tron_match(int mode) {
    video_info_t v; 
    get_video_data(&v);
    uint16_t W = v.width, H = v.height;

    p1.score = 0;
    p2.score = 0;

    int box_size = 10;
    int margin = 5;

    while (p1.score < 3 && p2.score < 3) {
        clear_screen();
        draw_border();

        // Marcador visible (cuadrados por victoria)
        for (int i = 0; i < p1.score; i++)
            video_draw_rect(10 + i*(box_size+margin), 10, box_size, box_size, 0xFF0000);
        for (int i = 0; i < p2.score; i++)
            video_draw_rect(W - (10 + (i+1)*(box_size+margin)), 10, box_size, box_size, 0x0000FF);

        // Inicialización de jugadores
        init_player(&p1, &p2);
        


        draw_head(&p1);
        draw_head(&p2);
        kbd_drain();

        // Bucle de una ronda
        while (p1.alive && p2.alive) {
            int k = getchar_nb();
            if (k >= 0) {
                if ((k=='w'||k=='W') && p1.dir!='D') p1.dir='U';
                else if ((k=='s'||k=='S') && p1.dir!='U') p1.dir='D';
                else if ((k=='a'||k=='A') && p1.dir!='R') p1.dir='L';
                else if ((k=='d'||k=='D') && p1.dir!='L') p1.dir='R';
                else if (k=='q'||k=='Q') return 'q';
            }

            if (mode == 2) {
                if ((k=='i'||k=='I') && p2.dir!='D') p2.dir='U';
                else if ((k=='k'||k=='K') && p2.dir!='U') p2.dir='D';
                else if ((k=='j'||k=='J') && p2.dir!='R') p2.dir='L';
                else if ((k=='l'||k=='L') && p2.dir!='L') p2.dir='R';
            } else {
                cpu_move(&p2);
            }

            move_player(&p1);
            move_player(&p2);

            if (check_border_collision(&p1, W, H)) p1.alive = 0;
            if (check_border_collision(&p2, W, H)) p2.alive = 0;

            if (check_collision(p1.x, p1.y, p1.dir, p2.color, p1.color)) p1.alive = 0;
            if (check_collision(p2.x, p2.y, p2.dir, p1.color, p2.color)) p2.alive = 0;

            if (p1.alive) draw_head(&p1);
            if (p2.alive) draw_head(&p2);

            wait_tick();
        }

        // Resultado de ronda
        if (p1.alive && !p2.alive) {
            p1.score++;
            video_draw_rect(10 + (p1.score-1)*(box_size+margin), 10, box_size, box_size, 0xFF0000);
        } else if (!p1.alive && p2.alive) {
            p2.score++;
            video_draw_rect(W - (10 + p2.score*(box_size+margin)), 10, box_size, box_size, 0x0000FF);
        }

        for (volatile int i = 0; i < 20000000; i++);


    }
    clear_screen();
    victory_screen( &p1, &p2, &current_level);
        puts(p1.matches_won);
        puts(p2.matches_won);    
    puts("Presiona cualquier tecla para continuar...");
    return getchar();
}

void tron_level(int mode){
    char c = tron_match(mode);
    while( c != 'q' && c != 'Q' ) {

        c = tron_match(mode);
    }
}

void tron_main(void) {
    int mode = select_mode();
    if (mode == 3) return;

    p1.matches_won = 0; p2.matches_won = 0;
    current_level = 0;
    tron_level(mode);
}


/*
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

        if (mode == 3) { return; }
        

        move_player(&p1);

        if (mode == 1) {
            cpu_move(&p2);      // Cambia dirección aleatoriamente
            move_player(&p2);   // AHORA SÍ SE MUEVE
        } else {
            move_player(&p2);   // Modo PvP
        }

        // Colisiones - USANDO LA NUEVA FUNCIÓN
        if (check_border_collision(&p1, W, H)) p1.alive = 0;
        if (check_border_collision(&p2, W, H)) p2.alive = 0;
        
        // Reemplazar las llamadas antiguas con la nueva función
        if (check_collision(p1.x, p1.y, p1.dir, p2.color, p1.color)) p1.alive = 0;
        if (check_collision(p2.x, p2.y, p2.dir, p1.color, p2.color)) p2.alive = 0;

        // Eliminar esta línea ya que está duplicada con las anteriores
        // if (p1.alive && p2.alive && check_player_collision(&p1, &p2)) { p1.alive=0; p2.alive=0; }

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
}*/