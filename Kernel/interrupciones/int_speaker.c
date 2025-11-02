#include "speaker.h"
#include "lib.h"        
#include "io.ports.h"
#include "time.h"      // si tenés funciones para esperar ticks
#include <stdint.h>

#define PIT_CHANNEL_2 0x42
#define PIT_COMMAND   0x43
#define SPEAKER_PORT  0x61
#define PIT_BASE_FREQUENCY 1193180  // Hz

// Envía un comando al PIT para establecer frecuencia
static void pit_set_frequency(uint32_t freq) {
    if (freq == 0)
        freq = 1; // evitar división por cero

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / freq);

    // Modo 3 (square wave), canal 2
    outb(PIT_COMMAND, 0xB6);
    outb(PIT_CHANNEL_2, (uint8_t)(divisor & 0xFF));       // Low byte
    outb(PIT_CHANNEL_2, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
}

// Enciende el speaker (habilita bit 0 y 1 del puerto 0x61)
static void speaker_on(void) {
    uint8_t tmp = inb(SPEAKER_PORT);
    if (tmp != (tmp | 3)) {
        outb(SPEAKER_PORT, tmp | 3);
    }
}

// Apaga el speaker (limpia bits 0 y 1)
void speaker_stop(void) {
    uint8_t tmp = inb(SPEAKER_PORT) & 0xFC;
    outb(SPEAKER_PORT, tmp);
}

// Reproduce un tono con frecuencia y duración especificadas
void speaker_play_tone(uint32_t freq, uint32_t duration_ms) {
    if (freq < 18 || freq > 20000) 
        return;

    pit_set_frequency(freq);
    speaker_on();

    // Esperar duración (usa sleep o busy-wait en ticks)
    sleep_ms(duration_ms);

    speaker_stop();
}