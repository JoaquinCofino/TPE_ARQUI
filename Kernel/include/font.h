#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// Bitmap font info struct def.
typedef struct {
    int width;
    int height;
    int nChars;
} bmp_font_inf;

// Información del font
extern bmp_font_inf FuenteTPE_16_inf;

// Array del bitmap font (8 pixels de ancho, 17 de alto, 256 caracteres)
extern const unsigned char FuenteTPE_16_bmp[][17];

#endif