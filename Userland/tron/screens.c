#include "../include/stdio.h"
#include "../include/syscall.h"
#include "../include/tron.h"
#include <stdint.h>
#include "../include/screens.h"

// Helpers de dibujo
static inline void hbar(int x, int y, int w, int t, uint32_t color) {
    video_draw_rect(x, y, w, t, color);
}

static inline void vbar(int x, int y, int h, int t, uint32_t color) {
    video_draw_rect(x, y, t, h, color);
}

static inline void pixel(int x, int y, uint32_t color) {
    video_putpixel((uint16_t)x, (uint16_t)y, color);
}

// Reemplazo de abs (no dependemos de libc)
static inline int iabs(int x) { 
    return x < 0 ? -x : x; 
}

// Línea diagonal gruesa simple (aprox) entre (x1,y1)-(x2,y2)
static void thick_line(int x1, int y1, int x2, int y2, int t, uint32_t color) {
    int dx = x2 - x1, dy = y2 - y1;
    int adx = iabs(dx), ady = iabs(dy);
    int steps = (adx > ady ? adx : ady);
    if (steps <= 0) steps = 1;

    for (int i = 0; i <= steps; i++) {
        int x = x1 + (dx * i) / steps;
        int y = y1 + (dy * i) / steps;
        video_draw_rect(x - t/2, y - t/2, t, t, color);
    }
}

// Dibuja una letra grande (solo T,R,O,N soportadas)
static void draw_big_char(int x, int y, int cw, int ch, int thick, char c, uint32_t color) {
    int w = cw, h = ch, t = thick;
    switch (c) {
        case 'T':
            hbar(x, y, w, t, color);
            vbar(x + w/2 - t/2, y, h, t, color);
            break;
        case 'R': {
            // Cuerpo
            vbar(x, y, h, t, color);                                       // barra izquierda
            hbar(x, y, w - t, t, color);                                   // barra superior
            hbar(x, y + h/2 - t/2, w - t, t, color);                      // barra media
            vbar(x + w - t, y, h/2, t, color);                            // barra derecha superior
            // pierna diagonal
            thick_line(x + w/2, y + h/2, x + w - t, y + h, t, color);
            break;
        }
        case 'O':
            hbar(x, y, w, t, color);                                       // top
            hbar(x, y + h - t, w, t, color);                              // bottom
            vbar(x, y, h, t, color);                                      // left
            vbar(x + w - t, y, h, t, color);                              // right
            break;
        case 'N':
            vbar(x, y, h, t, color);                                      // left
            vbar(x + w - t, y, h, t, color);                              // right
            thick_line(x + t, y + t, x + w - t, y + h - t, t, color);     // diagonal
            break;
        case ' ':
            // espacio: no dibuja
            break;
        default:
            // fallback: rectángulo sólido
            video_draw_rect(x, y, w, h, color);
            break;
    }
}

static void draw_small_char(int x, int y, int w, int h, int t, char c, uint32_t color) {
    // Posiciones auxiliares consistentes
    int left  = x;
    int right = x + w - t;
    int midx  = x + w/2 - t/2;
    int top   = y;
    int mid   = y + h/2 - t/2;
    int bot   = y + h - t;

    // Evitar tamaños negativos
    if (w <= 0 || h <= 0 || t <= 0) return;

    switch (c) {
        case 'a':
            // a minúscula (versión mejorada)
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, mid, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case 'b':
            // barra izquierda completa + tres barras horizontales y semicolumna derecha
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            hbar(left, bot, w - t, t, color);
            vbar(right, top, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case 'c':
            // c minúscula
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, mid, h/2, t, color);
            break;

        case 'd':
            // 'd' con cuerpo derecho y base más redondeada
            vbar(x + w - t, y, h, t, color);                              // borde derecho
            hbar(x + t, y + h/2, w - 2*t, t, color);                      // línea media
            hbar(x + t, y + h - t, w - 2*t, t, color);                    // base
            vbar(x, y + h/2, h/2, t, color);                              // lado izquierdo
            break;

        case 'e':
            // e minúscula
            hbar(left + t, top + t, w - 2*t, t, color);
            vbar(left, top + t, h - 2*t, t, color);
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            break;

        case 'h':
            // h minúscula
            vbar(left, top, h, t, color);
            vbar(right, mid, h/2, t, color);
            hbar(left, mid, w, t, color);
            break;

        case 'i':
            // i minúscula: barra vertical centrada y punto arriba
            vbar(x + w/2 - t/2, y + h/4, h * 3/4 - t, t, color);          // cuerpo
            // punto claramente separado arriba
            hbar(x + w/2 - t/2, y + h/8, t, t, color);
            break;

        case 'l': 
            // 'l' más angosta, sin espacio extra
            vbar(x + w/2 - t/2, y, h, t, color);
            break;

        case 'm': 
            // m minúscula
            vbar(left, top, h, t, color);
            vbar(left + w/3, top, h, t, color);
            vbar(left + 2*w/3, top, h, t, color);
            vbar(right, top, h, t, color);
            hbar(left, top, w, t, color);
            break;

        case 'n':
            // n minúscula
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            hbar(left, top, w, t, color);
            break;

        case 'o': 
            // o minúscula (círculo completo)
            hbar(left + t, top + t, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, top + t, h - 2*t, t, color);
            vbar(right, top + t, h - 2*t, t, color);
            break;

        case 'p':
            // p minúscula
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            vbar(right, top, h/2, t, color);
            break;

        case 'r':
            // r minúscula
            vbar(left, top, h, t, color);
            hbar(left, mid, w/2, t, color);
            break;

        case 's':
            // s minúscula
            hbar(left, top + t, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top + t, h/2 - t, t, color);
            vbar(right, mid, h/2 - t, t, color);
            break;

        case 't':
            // t minúscula (versión mejorada)
            vbar(midx, top + h/4, h*3/4, t, color);
            hbar(left, top + h/4 + t, w, t, color);
            break;

        case 'u':
            // u minúscula
            vbar(left, top, h - t, t, color);
            vbar(right, top, h - t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'v':
            // v minúscula
            vbar(left, top, h*2/3, t, color);
            vbar(right, top, h*2/3, t, color);
            hbar(x + w/4, bot, w/2, t, color);
            break;

        case 'y':
            // y minúscula
            vbar(left, top + h/3, h/2, t, color);
            vbar(right, top + h/3, h/2, t, color);
            vbar(midx, top + h/2, h/2, t, color);
            break;

        case 'M':
            // M mayúscula
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            thick_line(left, top, left + w/2, top + h/2, t, color);
            thick_line(left + w/2, top + h/2, right, top, t, color);
            break;

        case 'C':
            // C mayúscula
            hbar(left, top, w, t, color);
            vbar(left, top, h, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'P':
            // P mayúscula
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            vbar(right, top, h/2, t, color);
            break;

        case 'Q':
            // Q mayúscula
            hbar(left, top, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            thick_line(right - t, bot - t, right + t, bot + t, t, color);
            break;

        case 'U':
            // U mayúscula
            vbar(left, top, h - t, t, color);
            vbar(right, top, h - t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case '1':
            // Número 1
            vbar(midx, top, h, t, color);
            break;

        case '2':
            // Número 2
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(right, top, h/2, t, color);
            vbar(left, mid, h/2, t, color);
            break;

        case '-':
            // Guión
            hbar(left, mid, w, t, color);
            break;

        case '(':
            // Paréntesis izquierdo
            vbar(x + t, y + t, h - 2*t, t, color);
            hbar(x + t, y, w/2, t, color);
            hbar(x + t, y + h - t, w/2, t, color);
            break;

        case ')':
            // Paréntesis derecho
            vbar(x + 2*w/3, y, h, t, color);
            hbar(x + w/3, y, w/3, t, color);
            hbar(x + w/3, y + h - t, w/3, t, color);
            break;

        case ' ':
            // Espacio: no dibujar
            break;

        default:
            // Fallback: dibuja borde y relleno ligero para visibilidad
            hbar(left, top, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            break;
    }
}

static void draw_small_text(int x, int y, int char_w, int char_h, int thick, int spacing, const char *text, uint32_t color) {
    int cur_x = x;
    for (const char *p = text; *p; ++p) {
        draw_small_char(cur_x, y, char_w, char_h, thick, *p, color);
        cur_x += char_w + spacing;
    }
}

static void draw_big_text_centered(const char *s, int char_w, int char_h, int thick, int spacing, uint32_t color) {
    video_info_t v; 
    get_video_data(&v);
    int len = 0; 
    for (const char *p = s; *p; ++p) len++;
    int total_w = len * char_w + (len > 0 ? (len - 1) * spacing : 0);
    int start_x = (int)v.width / 2 - total_w / 2;
    int start_y = (int)v.height / 2 - char_h / 2;

    int x = start_x;
    for (const char *p = s; *p; ++p) {
        draw_big_char(x, start_y, char_w, char_h, thick, *p, color);
        x += char_w + spacing;
    }
}

void mode_screen(){
    clear_screen();
    
    // Aumentar el tamaño de la fuente para el título - MUY GRANDE
    syscall_increase_font_scale();
    syscall_increase_font_scale();
    syscall_increase_font_scale();
    syscall_increase_font_scale();
    syscall_increase_font_scale();
    
    // Centrar TRON verticalmente y horizontalmente
    
    // Reducir fuente para las opciones (tamaño normal)
    puts("\n\n\n\n\n\n\n\n");
    puts("                        TRON");
    puts("\n\n\n\n");
    puts("                   Seleccionar modo:");
    puts("                   1 - Player vs CPU");
    puts("                   2 - Player vs Player");
    puts("                   Q - Salir");
}

void victory_screen(Player *p1, Player *p2) {
    video_info_t v;
    get_video_data(&v);
    clear_screen();

    // --- Determinar ganador ---
    Player *winner = (p1->score == 3) ? p1 : p2;
    Player *loser  = (p1->score == 3) ? p2 : p1;

    winner->matches_won++;

    // --- Dibujar cuadrado del ganador ---
    int rect_w = 100, rect_h = 100;
    int rect_x = v.width / 2 - rect_w / 2;
    int rect_y = v.height / 3 - rect_h / 2; // Posicionado más arriba como mode_screen
    video_draw_rect(rect_x, rect_y, rect_w, rect_h, winner->color);

    // --- Texto usando el mismo estilo que mode_screen ---
    const char *line1 = "Match score";
    
    // Crear string "A - B" con las partidas ganadas
    char line2[16];
    line2[0] = (char)('0' + (p1->matches_won % 10));
    line2[1] = ' ';
    line2[2] = '-';
    line2[3] = ' ';
    line2[4] = (char)('0' + (p2->matches_won % 10));
    line2[5] = '\0';

    // --- Parámetros de texto similares a mode_screen ---
    int char_w = v.width / 30;
    int char_h = v.height / 20;
    int thick = char_w / 6;
    int spacing = char_w / 5;

    if (char_w < 12) char_w = 12;
    if (char_h < 20) char_h = 20;
    if (thick < 3) thick = 3;

    // --- Calcular posiciones centradas ---
    int len1 = 0, len2 = 0;
    for (const char *p = line1; *p; ++p) len1++;
    for (const char *p = line2; *p; ++p) len2++;

    int total_w1 = len1 * char_w + (len1 > 0 ? (len1 - 1) * spacing : 0);
    int total_w2 = len2 * char_w + (len2 > 0 ? (len2 - 1) * spacing : 0);

    int center_x1 = v.width / 2 - total_w1 / 2;
    int center_x2 = v.width / 2 - total_w2 / 2;

    // --- Posición del texto debajo del cuadrado ---
    int text_y1 = rect_y + rect_h + 40; // Espacio después del cuadrado
    int text_y2 = text_y1 + char_h + 20; // Espacio entre líneas

    uint32_t color = 0xFFFFFF;

    // --- Dibujar texto línea por línea ---
    int x = center_x1;
    for (const char *p = line1; *p; ++p) {
        draw_small_char(x, text_y1, char_w, char_h, thick, *p, color);
        x += char_w + spacing;
    }

    x = center_x2;
    for (const char *p = line2; *p; ++p) {
        draw_small_char(x, text_y2, char_w, char_h, thick, *p, color);
        x += char_w + spacing;
    }
}