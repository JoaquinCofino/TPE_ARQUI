#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

// Funciones básicas de pixel
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

// Funciones de dibujo de caracteres usando bitmap font
void drawChar(char c, uint64_t x, uint64_t y, uint32_t color);
void drawString(const char *s, uint64_t x, uint64_t y, uint32_t color);

// Funciones para obtener dimensiones del font
uint8_t getFontWidth(void);
uint8_t getFontHeight(void);

// Funciones para obtener dimensiones de pantalla
uint16_t getScreenWidth(void);
uint16_t getScreenHeight(void);
void initializeScreen(void);
void videoClear(void);

#endif