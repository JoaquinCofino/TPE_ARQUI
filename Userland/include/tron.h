#ifndef TRON_H
#define TRON_H

#include <stdint.h>


#define TICK_DELAY 800000  // ajusta velocidad

typedef struct {
    int x, y;
    char dir;     // 'U','D','L','R'
    int alive;
    uint32_t color;
} Player;


void init_game();
void update_game();
void render_game();
void handle_input();
void tron_main();

#endif // TRON_H