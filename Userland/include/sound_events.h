#ifndef SOUND_EVENTS_H
#define SOUND_EVENTS_H

// Tipos de sonidos para el juego Tron
typedef enum {
    SND_NONE = 0,    // silencio / sin sonido
    SND_START,       // inicio de partida
    SND_MOVE,        // movimiento
    SND_CRASH,       // colisión
    SND_WIN,         // victoria
    SND_LOSE         // derrota
} sound_event_t;

#endif