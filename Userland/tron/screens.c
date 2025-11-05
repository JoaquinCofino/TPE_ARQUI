#include "../include/stdio.h"
#include "../include/syscall.h"
#include "../include/tron.h"
#include <stdint.h>
#include "../include/screens.h"

static void draw_small_char(int x, int y, int w, int h, int t, char c, uint32_t color) ;

static inline void hbar(int x, int y, int w, int t, uint32_t color) {
    video_draw_rect(x, y, w, t, color);
}

static inline void vbar(int x, int y, int h, int t, uint32_t color) {
    video_draw_rect(x, y, t, h, color);
}

static inline void pixel(int x, int y, uint32_t color) {
    video_putpixel((uint16_t)x, (uint16_t)y, color);
}

static inline int iabs(int x) { 
    return x < 0 ? -x : x; 
}

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

static void draw_big_char(int x, int y, int cw, int ch, int thick, char c, uint32_t color) {
    int w = cw, h = ch, t = thick;

    if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ' || c == '-' || c == '(' || c == ')') {
        draw_small_char(x, y, w, h, t, c, color);
        return;
    }

    switch (c) {
        case 'T':
            hbar(x, y, w, t, color);
            vbar(x + w/2 - t/2, y, h, t, color);
            break;
        case 'R': {
            vbar(x, y, h, t, color);                                       
            hbar(x, y, w - t, t, color);                                   
            hbar(x, y + h/2 - t/2, w - t, t, color);                     
            vbar(x + w - t, y, h/2, t, color);                           
            // pierna diagonal
            thick_line(x + w/2, y + h/2, x + w - t, y + h, t, color);
            break;
        }
        case 'O':
            hbar(x, y, w, t, color);                                      
            hbar(x, y + h - t, w, t, color);                              
            vbar(x, y, h, t, color);                                      
            vbar(x + w - t, y, h, t, color);                              
            break;
        case 'N':
            vbar(x, y, h, t, color);                                      
            vbar(x + w - t, y, h, t, color);                              
            thick_line(x + t, y + t, x + w - t, y + h - t, t, color);     
            break;
        case ' ':
            break;
        default:
            video_draw_rect(x, y, w, h, color);
            break;
    }
}

static void draw_small_char(int x, int y, int w, int h, int t, char c, uint32_t color) {
    int left  = x;
    int right = x + w - t;
    int midx  = x + w/2 - t/2;
    int top   = y;
    int mid   = y + h/2 - t/2;
    int bot   = y + h - t;

    if (w <= 0 || h <= 0 || t <= 0) return;

    switch (c) {
        case 'a':
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, mid, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case 'b':
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            hbar(left, bot, w - t, t, color);
            vbar(right, top, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case 'c':
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, mid, h/2, t, color);
            break;

        case 'd':
            vbar(x + w - t, y, h, t, color);                            
            hbar(x + t, y + h/2, w - 2*t, t, color);                     
            hbar(x + t, y + h - t, w - 2*t, t, color);                  
            vbar(x, y + h/2, h/2, t, color);                             
            break;

        case 'e':
            hbar(left + t, top + t, w - 2*t, t, color);
            vbar(left, top + t, h - 2*t, t, color);
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            break;

        case 'h':
            vbar(left, top, h, t, color);
            vbar(right, mid, h/2, t, color);
            hbar(left, mid, w, t, color);
            break;

        case 'i':
            vbar(x + w/2 - t/2, y + h/4, h * 3/4 - t, t, color);          
            hbar(x + w/2 - t/2, y + h/8, t, t, color);
            break;

        case 'l': 
            vbar(x + w/2 - t/2, y, h, t, color);
            break;

        case 'm': 
            vbar(left, top, h, t, color);
            vbar(left + w/3, top, h, t, color);
            vbar(left + 2*w/3, top, h, t, color);
            vbar(right, top, h, t, color);
            hbar(left, top, w, t, color);
            break;

        case 'n':
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            hbar(left, top, w, t, color);
            break;

        case 'o': 
            hbar(left + t, top + t, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, top + t, h - 2*t, t, color);
            vbar(right, top + t, h - 2*t, t, color);
            break;

        case 'p':
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            vbar(right, top, h/2, t, color);
            break;

        case 'r':
            vbar(left, top, h, t, color);
            hbar(left, mid, w/2, t, color);
            break;

        case 's':
            hbar(left, top + t, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top + t, h/2 - t, t, color);
            vbar(right, mid, h/2 - t, t, color);
            break;

        case 't':
            vbar(midx, top + h/4, h*3/4, t, color);
            hbar(left, top + h/4 + t, w, t, color);
            break;

        case 'u':
            vbar(left, top, h - t, t, color);
            vbar(right, top, h - t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'v':
            vbar(left, top, h*2/3, t, color);
            vbar(right, top, h*2/3, t, color);
            hbar(x + w/4, bot, w/2, t, color);
            break;

        case 'y':
            vbar(left, top + h/3, h/2, t, color);
            vbar(right, top + h/3, h/2, t, color);
            vbar(midx, top + h/2, h/2, t, color);
            break;

        case 'M':
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            thick_line(left, top, left + w/2, top + h/2, t, color);
            thick_line(left + w/2, top + h/2, right, top, t, color);
            break;

        case 'C':
            hbar(left, top, w, t, color);
            vbar(left, top, h, t, color);
            hbar(left, bot, w, t, color);
            break;

        case 'P':
            vbar(left, top, h, t, color);
            hbar(left, top, w - t, t, color);
            hbar(left, mid, w - t, t, color);
            vbar(right, top, h/2, t, color);
            break;

        case 'Q':
            hbar(left, top, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            thick_line(right - t, bot - t, right + t, bot + t, t, color);
            break;

        case 'U':
            vbar(left, top, h - t, t, color);
            vbar(right, top, h - t, t, color);
            hbar(left, bot, w, t, color);
            break;

        case '-':
            hbar(left, mid, w, t, color);
            break;

        case '(':
            vbar(x + t, y + t, h - 2*t, t, color);
            hbar(x + t, y, w/2, t, color);
            hbar(x + t, y + h - t, w/2, t, color);
            break;

        case ')':
            vbar(x + 2*w/3, y, h, t, color);
            hbar(x + w/3, y, w/3, t, color);
            hbar(x + w/3, y + h - t, w/3, t, color);
            break;
        case 'g':
            hbar(left + t, top + t, w - 2*t, t, color);          
            hbar(left + t, mid, w - 2*t, t, color);
            hbar(left + t, bot, w - 2*t, t, color);
            vbar(left, top + t, h - 2*t, t, color);
            vbar(right, top + t, (h/2) + t, t, color);           
            {
                int cx1 = x + w/2;                               
                int cy1 = y + mid + t/2;
                int cx2 = x + w - t;                          
                int cy2 = y + h - t;
                thick_line(cx1, cy1, cx2, cy2, t, color);
                hbar(cx2 - w/4, cy2 - t/2, w/4, t, color);
            }
            break;

        case ' ':
           
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

        case '3':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(right, top, h, t, color);
            break;

        case '4':
            vbar(left, top, h/2, t, color);
            vbar(right, top, h, t, color);
            hbar(left, mid, w, t, color);
            break;

        case '5':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h/2, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case '6':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, mid, h/2, t, color);
            break;

        case '7':
            hbar(left, top, w, t, color);
            vbar(right, top, h, t, color);
            break;

        case '8':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            break;

        case '9':
            hbar(left, top, w, t, color);
            hbar(left, mid, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h/2, t, color);
            vbar(right, top, h, t, color);
            break;

        case '0':
            hbar(left, top, w, t, color);
            hbar(left, bot, w, t, color);
            vbar(left, top, h, t, color);
            vbar(right, top, h, t, color);
            break;


        default:
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
    video_info_t v; 
    get_video_data(&v);
    clear_screen();

    int cw = v.width / 12;                                                 
    int ch = v.height / 6;                                                 
    if (cw < 30) cw = 30;
    if (ch < 50) ch = 50;
    int thick = cw / 6;                                                    
    if (thick < 4) thick = 4;
    int spacing = cw / 5;

    int len = 4;                                                           
    int total_w = len * cw + (len - 1) * spacing;
    int start_x = (int)v.width / 2 - total_w / 2;
    int start_y = (int)v.height / 3 - ch / 2;                            

    int x = start_x;
    const char *tron = "TRON";
    for (const char *p = tron; *p; ++p) {
        draw_big_char(x, start_y, cw, ch, thick, *p, 0xFFFFFF);
        x += cw + spacing;
    }

    int small_w = cw / 4;                                                
    int small_h = ch / 4;                                                
    int small_t = thick / 2;
    int small_spacing = small_w / 6;                                      

    int base_y = start_y + ch + ch/2;                                      
    
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


void victory_screen(Player *p1, Player *p2, int * current_level) {
    video_info_t v;
    get_video_data(&v);
    clear_screen();

    Player *winner = (p1->score == 3) ? p1 : p2;
    Player *loser  = (p1->score == 3) ? p2 : p1;

    winner->matches_won++;

    if ((winner->score % 3) == 0) {
        (*current_level)++;
    }

    int rect_w = 100, rect_h = 100;
    int rect_x = v.width / 2 - rect_w / 2;
    int rect_y = v.height / 3 - rect_h / 2;
    video_draw_rect(rect_x, rect_y, rect_w, rect_h, winner->color);

    const char *line1 = "Match score";

    char line2[16];
    line2[0] = (char)('0' + (p1->matches_won % 10));
    line2[1] = ' ';
    line2[2] = '-';
    line2[3] = ' ';
    line2[4] = (char)('0' + (p2->matches_won % 10));
    line2[5] = '\0';

    int char_w = v.width / 30;
    int char_h = v.height / 20;
    int thick = char_w / 6;
    int spacing = char_w / 5;

    if (char_w < 12) char_w = 12;
    if (char_h < 20) char_h = 20;
    if (thick < 3) thick = 3;

    int len1 = 0, len2 = 0;
    for (const char *p = line1; *p; ++p) len1++;
    for (const char *p = line2; *p; ++p) len2++;

    int total_w1 = len1 * char_w + (len1 > 0 ? (len1 - 1) * spacing : 0);
    int total_w2 = len2 * char_w + (len2 > 0 ? (len2 - 1) * spacing : 0);

    int center_x1 = v.width / 2 - total_w1 / 2;
    int center_x2 = v.width / 2 - total_w2 / 2;

    int text_y1 = rect_y + rect_h + 40; 
    int text_y2 = text_y1 + char_h + 20; 

    uint32_t color = 0xFFFFFF;

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


        char level_text[8];
        int lvl = *current_level;
        int pos = 0;

        const char *prefix = "nivel ";
        for (const char *pp = prefix; *pp; ++pp) level_text[pos++] = *pp;

        if (lvl < 10) {
            level_text[pos++] = '0' + lvl;
        } else {
            level_text[pos++] = '0' + (lvl / 10) % 10;
            level_text[pos++] = '0' + (lvl % 10);
        }
        level_text[pos] = '\0';

        int len3 = 0;
        for (const char *p = level_text; *p; ++p) len3++;
        int total_w3 = len3 * char_w + (len3 > 0 ? (len3 - 1) * spacing : 0);
        int center_x3 = v.width / 2 - total_w3 / 2;

        int text_y3 = text_y2 + char_h + 20; 

        int lx = center_x3;
        for (const char *p = level_text; *p; ++p) {
            draw_small_char(lx, text_y3, char_w, char_h, thick, *p, color);
            lx += char_w + spacing;
        }

}

void finalWin_screen(Player *p1, Player *p2) {
    video_info_t v;
    get_video_data(&v);
    clear_screen();

    Player *winner = (p1->matches_won > p2->matches_won) ? p1 : p2;
    int is_tie = (p1->matches_won == p2->matches_won);

    int cw = v.width / 12;
    int ch = v.height / 6;
    if (cw < 30) cw = 30;
    if (ch < 50) ch = 50;
    int thick = cw / 6;
    if (thick < 4) thick = 4;
    int spacing = cw / 5;

    const char *title_text = is_tie ? "empate" : "ganador";

    int len_title = 0;
    for (const char *p = title_text; *p; ++p) len_title++;
    int total_title_w = len_title * cw + (len_title - 1) * spacing;
    int title_x = v.width / 2 - total_title_w / 2;
    int title_y = v.height / 4 - ch / 2;

    int x = title_x;
    for (const char *p = title_text; *p; ++p) {
        draw_big_char(x, title_y, cw, ch, thick, *p, 0xFFFFFF);
        x += cw + spacing;
    }

    int rect_y = title_y + ch + 20;
    if (!is_tie) {
        int rect_w = 100, rect_h = 100;
        int rect_x = v.width / 2 - rect_w / 2;
        video_draw_rect(rect_x, rect_y, rect_w, rect_h, winner->color);
    }

    int small_w = cw / 4;
    int small_h = ch / 4;
    int small_t = thick / 2;
    int small_spacing = small_w / 6;

    const char *label1 = "player1 ";
    const char *label2 = "player2 ";

    int label1_len = 0, label2_len = 0;
    for (const char *pp = label1; *pp; ++pp) label1_len++;
    for (const char *pp = label2; *pp; ++pp) label2_len++;

    int label1_w = label1_len * (small_w + small_spacing) - small_spacing;
    int label2_w = label2_len * (small_w + small_spacing) - small_spacing;

    char score1_ch = '0' + (p1->matches_won % 10);
    char score2_ch = '0' + (p2->matches_won % 10);

    int gap_px = 2 * small_w;

    int total_w1_px = label1_w + gap_px + small_w;
    int total_w2_px = label2_w + gap_px + small_w;

    int start_x1 = v.width / 2 - total_w1_px / 2;
    int start_x2 = v.width / 2 - total_w2_px / 2;

    int base_y = rect_y + (is_tie ? 40 : 140);
    int score_y1 = base_y;
    int score_y2 = score_y1 + small_h * 2;

    draw_small_text(start_x1, score_y1, small_w, small_h, small_t, small_spacing, label1, 0xFFFFFF);
    {
        int score_x = start_x1 + label1_w + gap_px;
        char sc1[2] = { score1_ch, '\0' };
        draw_small_text(score_x, score_y1, small_w, small_h, small_t, small_spacing, sc1, 0xFFFFFF);
    }

    draw_small_text(start_x2, score_y2, small_w, small_h, small_t, small_spacing, label2, 0xFFFFFF);
    {
        int score_x = start_x2 + label2_w + gap_px;
        char sc2[2] = { score2_ch, '\0' };
        draw_small_text(score_x, score_y2, small_w, small_h, small_t, small_spacing, sc2, 0xFFFFFF);
    }

    const char *continue_text = "presiona tecla para continuar";
    int len_c = 0;
    for (const char *p = continue_text; *p; ++p) len_c++;
    int total_cw = len_c * (small_w + small_spacing) - small_spacing;
    int cont_x = v.width / 2 - total_cw / 2;
    int cont_y = score_y2 + small_h * 4;

    draw_small_text(cont_x, cont_y, small_w, small_h, small_t, small_spacing, continue_text, 0xFFFFFF);

    getchar();
    while (getchar_nb() >= 0) { }

    tron_main();
}
