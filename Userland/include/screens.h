#ifndef SCREENS_H
#define SCREENS_H

#include <stdint.h>
#include "tron.h"
void mode_screen();
void victory_screen(Player *p1, Player *p2, int * current_level);
void finalWin_screen(Player *p1, Player *p2);
#endif // SCREENS_H