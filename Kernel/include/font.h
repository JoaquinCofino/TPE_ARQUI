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

// Funciones para manejo de tamaño de fuente
void font_set_scale(int scale);
int font_get_scale(void);
void font_increase_size(void);
void font_decrease_size(void);
bmp_font_inf font_get_scaled_info(void);

// Función para obtener carácter escalado
void font_get_scaled_char(unsigned char c, unsigned char* buffer, int* width, int* height);


#endif