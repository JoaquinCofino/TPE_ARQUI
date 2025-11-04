#ifndef SONIDOS_H
#define SONIDOS_H

// Tipos de sonidos para el juego Tron
typedef enum {
    SND_NONE = 0,    // silencio / sin sonido
    SND_START,       // inicio de partida
    SND_MOVE,        // movimiento
    SND_CRASH,       // colisión
    SND_WIN,         // victoria
    SND_LOSE         // derrota
} sound_event_t;

// ============================================================================
// FRECUENCIAS DE NOTAS MUSICALES (en Hz)
// ============================================================================

// Octava 4 (media)
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494

// Octava 5 (alta)
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988

// Octava 6 (muy alta)
#define C6  1047
#define D6  1175
#define E6  1319
#define F6  1397
#define G6  1568
#define A6  1760

// Octava 3 (baja)
#define C3  131
#define D3  147
#define E3  165
#define F3  175
#define G3  196
#define A3  220
#define B3  247

// ============================================================================
// DURACIONES (en milisegundos)
// ============================================================================
#define WHOLE      1600  // Redonda
#define HALF       800   // Blanca
#define QUARTER    400   // Negra
#define EIGHTH     200   // Corchea
#define SIXTEENTH  100   // Semicorchea

// ============================================================================
// FUNCIONES DE MELODÍAS
// ============================================================================

// Melodía de arranque simple
void play_startup_sound(void) ;


// Star Wars - tema principal
void play_star_wars(void) ;

// Tetris - Tema A
void play_tetris(void);

// Nokia Ringtone
void play_nokia_ringtone(void) ;

// Sonido de error
void play_error_sound(void) ;

// Sonido de éxito
void play_success_sound(void) ;


// Sirena de policía
void play_siren(void) ;

// Escala ascendente
void play_scale_up(void);
// Efecto de laser
void play_laser(void);

// Efecto de explosión
void play_explosion(void);


#endif