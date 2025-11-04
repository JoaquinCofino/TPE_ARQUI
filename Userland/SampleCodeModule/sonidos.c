#include "sonidos.h"
#include "stdio.h"


void play_startup_sound(void) {
    puts(" Sonido de arranque...\n");
    play_sound(C4, EIGHTH);
    play_sound(E4, EIGHTH);
    play_sound(G4, EIGHTH);
    play_sound(C5, QUARTER);
}



// Star Wars - tema principal
void play_star_wars(void) {
    puts("Star Wars Theme!\n");
    play_sound(A4, QUARTER);
    play_sound(A4, QUARTER);
    play_sound(A4, QUARTER);
    play_sound(F4, EIGHTH + SIXTEENTH);
    play_sound(C5, SIXTEENTH);
    
    play_sound(A4, QUARTER);
    play_sound(F4, EIGHTH + SIXTEENTH);
    play_sound(C5, SIXTEENTH);
    play_sound(A4, HALF);
}

// Tetris - Tema A
void play_tetris(void) {
    puts("Tetris!\n");
    play_sound(E5, QUARTER);
    play_sound(B4, EIGHTH);
    play_sound(C5, EIGHTH);
    play_sound(D5, QUARTER);
    play_sound(C5, EIGHTH);
    play_sound(B4, EIGHTH);
    
    play_sound(A4, QUARTER);
    play_sound(A4, EIGHTH);
    play_sound(C5, EIGHTH);
    play_sound(E5, QUARTER);
    play_sound(D5, EIGHTH);
    play_sound(C5, EIGHTH);
    
    play_sound(B4, QUARTER);
    play_sound(C5, EIGHTH);
    play_sound(D5, QUARTER);
    play_sound(E5, QUARTER);
    play_sound(C5, QUARTER);
    play_sound(A4, QUARTER);
}

// Nokia Ringtone
void play_nokia_ringtone(void) {
    puts("Nokia Ringtone!\n");
    play_sound(E5, EIGHTH);
    play_sound(D5, EIGHTH);
    play_sound(FS4, QUARTER);
    play_sound(GS4, QUARTER);
    
    play_sound(CS5, EIGHTH);
    play_sound(B4, EIGHTH);
    play_sound(D4, QUARTER);
    play_sound(E4, QUARTER);
    
    play_sound(B4, EIGHTH);
    play_sound(A4, EIGHTH);
    play_sound(CS4, QUARTER);
    play_sound(E4, QUARTER);
    
    play_sound(A4, HALF);
}

// Sonido de error
void play_error_sound(void) {
    play_sound(200, 100);
    play_sound(150, 200);
}

// Sonido de éxito
void play_success_sound(void) {
    play_sound(C5, 100);
    play_sound(E5, 100);
    play_sound(G5, 100);
    play_sound(C6, 200);
}



// Sirena de policía
void play_siren(void) {
    for (int i = 0; i < 3; i++) {
        play_sound(1000, 300);
        play_sound(800, 300);
    }
}

// Escala ascendente
void play_scale_up(void) {
    puts(" Escala ascendente\n");
    play_sound(C4, EIGHTH);
    play_sound(D4, EIGHTH);
    play_sound(E4, EIGHTH);
    play_sound(F4, EIGHTH);
    play_sound(G4, EIGHTH);
    play_sound(A4, EIGHTH);
    play_sound(B4, EIGHTH);
    play_sound(C5, QUARTER);
}

// Efecto de laser
void play_laser(void) {
    for (int freq = 2000; freq > 200; freq -= 50) {
        play_sound(freq, 20);
    }
}

// Efecto de explosión
void play_explosion(void) {
    play_sound(100, 50);
    play_sound(80, 100);
    play_sound(60, 150);
}

// FUNCIONES DE SONIDOS PARA EL JUEGO TRON


//reproducir sonido del juego según evento
void play_game_sound(sound_event_t event) {
    switch (event) {
        case SND_CRASH:
            play_explosion();      
            break;
        case SND_WIN:
            play_success_sound();  
            break;
        case SND_LOSE:
            play_laser();    
            break;
        case SND_NONE:
        default:
            //silencio 
            break;
    }
}
