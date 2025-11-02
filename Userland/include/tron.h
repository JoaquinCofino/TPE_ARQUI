#ifndef TRON_H
#define TRON_H

#include <stdint.h>

typedef struct {
    int x;
    int y;
    char symbol;
    int alive;
    int dx;
    int dy;
} Player;


void init_game();
void update_game();
void render_game();
void handle_input();
void tron_main();

#endif // TRON_H