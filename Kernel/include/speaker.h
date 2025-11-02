#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>

void speaker_play_tone(uint32_t freq, uint32_t duration_ms);
void speaker_stop(void);

#endif