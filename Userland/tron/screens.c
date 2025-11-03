#include "../include/stdio.h"
#include "../include/syscall.h"
#include "../include/tron.h"
#include <stdint.h>

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
static inline int iabs(int x) { return x < 0 ? -x : x; }

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
            vbar(x, y, h, t, color);                         // barra izquierda
            hbar(x, y, w - t, t, color);                     // barra superior
            hbar(x, y + h/2 - t/2, w - t, t, color);         // barra media
            vbar(x + w - t, y, h/2, t, color);               // barra derecha superior
            // pierna diagonal
            thick_line(x + w/2, y + h/2, x + w - t, y + h, t, color);
            break;
        }
        case 'O':
            hbar(x, y, w, t, color);                         // top
            hbar(x, y + h - t, w, t, color);                 // bottom
            vbar(x, y, h, t, color);                         // left
            vbar(x + w - t, y, h, t, color);                 // right
            break;
        case 'N':
            vbar(x, y, h, t, color);                         // left
            vbar(x + w - t, y, h, t, color);                 // right
            thick_line(x + t, y + t, x + w - t, y + h - t, t, color); // diagonal
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

// Reemplazar esta función por la anterior: coloca rectángulos en posiciones calculadas
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
    // minúscula tipo 'a' abierta, no cerrada como 'o'
    hbar(x + t, y + h/2, w - 2*t, t, color);           // barra media
    hbar(x + t, y + h - t, w - 2*t, t, color);         // base
    vbar(x, y + h/2, h/2, t, color);                   // lado izquierdo
    vbar(x + w - t, y + h/2 - t, h/2, t, color);       // lado derecho
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
    // minúscula 'c' más clara, sin cierre completo
    hbar(x + t, y + h/2, w - 2*t, t, color);           // línea media
    hbar(x + t, y + h - t, w - 2*t, t, color);         // línea inferior
    vbar(x, y + h/2, h/2, t, color);                   // lado izquierdo
    break;

        case 'd':
    // 'd' con cuerpo derecho y base más redondeada
    vbar(x + w - t, y, h, t, color);                   // borde derecho
    hbar(x + t, y + h/2, w - 2*t, t, color);           // línea media
    hbar(x + t, y + h - t, w - 2*t, t, color);         // base
    vbar(x, y + h/2, h/2, t, color);                   // lado izquierdo
    break;


        case 'e':
    hbar(left + t, top + t, w - 2*t, t, color);
    vbar(left, top + t, h - 2*t, t, color);
    hbar(left + t, mid, w - 2*t, t, color);
    hbar(left + t, bot, w - 2*t, t, color);
            break;

        case 'i':
            // i minúscula: barra vertical centrada y punto arriba
    // todo calculado inline para evitar errores de redondeo
    vbar(x + w/2 - t/2, y + h/4, h * 3/4 - t, t, color);   // cuerpo

    // punto claramente separado arriba
    hbar(x + w/2 - t/2, y + h/8, t, t, color);
            break;

        case 'l': 
    // 'l' más angosta, sin espacio extra
    vbar(x + w/2 - t/2, y, h, t, color);
    break;

        case 'm': 
   // patas verticales
    vbar(left, y + h/2 - t/2, h/2, t, color);   // primera pata
    vbar(left + w/3, top + t, h/2, t, color); // segunda pata (más alta para dar forma)
    vbar(left + 2*w/3, top + t, h/2, t, color); // tercera pata
    vbar(left + w - t, y + h/2 - t/2, h/2, t, color); // última pata

    // unir cimas para formar la M
    hbar(left, top + t, w/3 + t, t, color);      // cima primera
    hbar(left + w/3, top + t, w/3 + t, t, color);      // cima segunda             // unión superior
    break;

        case 'n':
            // n: parecido a 'm' pero más simple
            vbar(left, mid, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            hbar(left, mid, w, t, color);
            break;

        case 'o': 
    // 'o' más simétrica (círculo cerrado)
    hbar(x + t, y + h/2, w - 2*t, t, color);           // medio
    hbar(x + t, y + h - t, w - 2*t, t, color);         // base
    vbar(x, y + h/2, h/2, t, color);                   // izquierda
    vbar(x + w - t, y + h/2, h/2, t, color);           // derecha
    break;

        case 'p':
            // p: similar a b pero sin la parte inferior completa
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            vbar(right, top, h/2, t, color);
            break;

        case 'r':
            // r: pequeño asta y pierna
            vbar(left, mid, h/2, t, color);
            hbar(left, mid, w/2 + t, t, color);
            break;

        case 's':
            // s: escalonado para legibilidad
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case 't':
            // t: columna central con barra inferior o superior según prefieras
            vbar(midx, top, h, t, color);
            hbar(left, top + t, w, t, color);
            break;

        case 'u':
            // u: columnas y barra inferior
            vbar(left, mid, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'y':
            // y: versión legible con cola
            vbar(left, top + h/3, h/2, t, color);
            vbar(right, top + h/3, h/2, t, color);
            vbar(midx, top + h/2, h/2, t, color);
            break;

        case '1':
            vbar(midx, top, h, t, color);
            break;

        case '2':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(right, top, h/2, t, color);
            vbar(left, mid, h/2, t, color);
            break;

        case 'v': // ya cubierto pero repetido no hace daño
            vbar(left, top + h/3, h*2/3, t, color);
            vbar(right, top + h/3, h*2/3, t, color);
            hbar(x + w/4, bot, w/2, t, color);
            break;

        case 'P':  // P mayúscula
            if (c == 'P') {
                vbar(left, top, h, t, color);
                hbar(left, top, w - t, t, color);
                hbar(left, mid, w - t, t, color);
                vbar(right, top, h/2, t, color);
            } else {
                // ya manejado arriba
                vbar(left, top, h, t, color);
                hbar(left, top, w - t, t, color);
                hbar(left, mid, w - t, t, color);
            }
            break;

        case 'C':
            hbar(left, top, w, t, color);
            vbar(left, mid, h - 2*t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'U':
            vbar(left, top, h - t, t, color);
            vbar(right, top, h - t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'Q':
        // Caja de la Q
        hbar(left, top, w, t, color);   // borde superior
        hbar(left, bot, w, t, color);   // borde inferior
    vbar(left, top, h, t, color);   // borde izquierdo
    vbar(right, top, h, t, color);  // borde derecho

    // Cola diagonal
    thick_line(right - t, bot - t, right + t, bot + t, t, color);
    break;

case 'q':

    // Círculo inferior de la q
    hbar(left, mid, w, t, color);
    hbar(left, bot, w, t, color);
    vbar(left, mid, h / 2, t, color);
    vbar(right, mid, h / 2, t, color);

    // Cola descendente
    vbar(right - t / 2, bot, h / 3, t, color);
    break;

case '(' : 
    // Paréntesis izquierdo curvado
    vbar(x + t, y + t, h - 2*t, t, color);
    hbar(x + t, y, w/2, t, color);
    hbar(x + t, y + h - t, w/2, t, color);
    break;

            
            case ')':
            // Paréntesis derecho: simétrico al anterior
            vbar(x + 2*w/3, y, h, t, color);
            hbar(x + w/3, y, w/3, t, color);           // borde superior
            hbar(x + w/3, y + h - t, w/3, t, color);   // borde inferior
            break;


        case ' ':
            // espacio: no dibujar
            break;

        default:
            // fallback claro: dibuja borde y relleno ligero para visibilidad
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
    video_info_t v; get_video_data(&v);
    int len = 0; for (const char *p = s; *p; ++p) len++;
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
    video_info_t v; get_video_data(&v);
    clear_screen();

    // Texto grande centrado: "TRON"
    // Ajustá tamaño/grueso según resolución
    int cw = v.width / 12;    // ancho de caracter
    int ch = v.height / 6;    // alto de caracter
    if (cw < 30) cw = 30;
    if (ch < 50) ch = 50;
    int thick = cw / 6;       // grosor de trazo
    if (thick < 4) thick = 4;
    int spacing = cw / 5;

 
// Posición más arriba - en lugar de centrado vertical, usar 1/3 de la pantalla
    int len = 4; // "TRON" tiene 4 letras
    int total_w = len * cw + (len - 1) * spacing;
    int start_x = (int)v.width / 2 - total_w / 2;
    int start_y = (int)v.height / 3 - ch / 2; // era v.height / 2, ahora v.height / 3

    int x = start_x;
    const char *tron = "TRON";
    for (const char *p = tron; *p; ++p) {
        draw_big_char(x, start_y, cw, ch, thick, *p, 0xFFFFFF);
        x += cw + spacing;
    }

   // --- Texto de opciones - también más pequeño ---
    int small_w = cw / 4;     // era cw / 3, ahora cw / 4
    int small_h = ch / 4;     // era ch / 3, ahora ch / 4
    int small_t = thick / 2;
    int small_spacing = small_w / 6; // espaciado más pequeño

    int base_y = start_y + ch + ch/2; // posición relativa al TRON
    
    // Calcular ancho del texto más largo para centrar mejor
    const char *longest_text = "player 1 vs player 2 (2)";
    int longest_len = 0;
    for (const char *p = longest_text; *p; ++p) longest_len++;
    int text_width = longest_len * (small_w + small_spacing) - small_spacing;
    int center_x = v.width / 2 - text_width / 2;

    draw_small_text(center_x, base_y, small_w, small_h, small_t, small_spacing, "seleccionar modo", 0xFFFFFF);
    draw_small_text(center_x, base_y + small_h * 3, small_w, small_h, small_t, small_spacing, "player 1 vs CPU (1)", 0xFFFFFF);
    draw_small_text(center_x, base_y + small_h * 5, small_w, small_h, small_t, small_spacing, "player 1 vs player 2 (2)", 0xFFFFFF);
    draw_small_text(center_x, base_y + small_h * 7, small_w, small_h, small_t, small_spacing, "salir (Q)", 0xFFFFFF);
}