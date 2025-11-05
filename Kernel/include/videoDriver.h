#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

// Funciones básicas de pixel
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
uint32_t getPixel(uint32_t x, uint32_t y);

// Funciones de dibujo de caracteres usando bitmap font
void drawChar(char c, uint64_t x, uint64_t y, uint32_t color);

// Funciones para obtener dimensiones del font
uint8_t getFontWidth(void);
uint8_t getFontHeight(void);

// Funciones para obtener dimensiones de pantalla
uint16_t getScreenWidth(void);
uint16_t getScreenHeight(void);
void initializeScreen(void);
void videoClear(void);
uint8_t getbpp(void);
void *getFramebuffer(void);
uint32_t getFramebufferPitch(void);

// Funciones de scroll
void scrollUpLines(uint32_t lines);

// Funciones BASE 
uint8_t getFontWidth(void);
uint8_t getFontHeight(void);

// Funciones ESCALADAS (solo para naiveConsole)
uint8_t getScaledFontWidth(void);
uint8_t getScaledFontHeight(void);

// Font scale management
uint8_t getFontScale(void);
void setFontScale(uint8_t scale);
void increaseFontScale(void);
void decreaseFontScale(void);
#endif