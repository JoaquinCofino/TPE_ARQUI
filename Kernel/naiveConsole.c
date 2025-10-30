#include <naiveConsole.h>
#include "videoDriver.h"
#include "font.h"

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };

// Variables para el cursor de texto (posición en caracteres)
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;

// Dimensiones de la consola en caracteres
static uint16_t maxCols = 0;
static uint16_t maxRows = 0;

// Color del texto
#define TEXT_COLOR 0xFFFFFF
#define BACKGROUND_COLOR 0x000000

// Inicializar las dimensiones de la consola
static void initConsole(void) {
    static int initialized = 0;
    if (!initialized) {
        maxCols = getScreenWidth() / getFontWidth();
        maxRows = getScreenHeight() / getFontHeight();
        initialized = 1;
    }
}

void ncPrint(const char * string) {
    int i;
    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}

void ncPrintChar(char character) {
    initConsole();
    
    // Verificar si necesitamos hacer scroll
    if (cursorY >= maxRows) {
        cursorY = 0;
        cursorX = 0;
    }
    
    // Verificar si llegamos al final de la línea
    if (cursorX >= maxCols) {
        ncNewline();
    }
    
    // Calcular posición en píxeles
    uint64_t pixelX = cursorX * getFontWidth();
    uint64_t pixelY = cursorY * getFontHeight();
    
    // Dibujar el carácter
    drawChar(character, pixelX, pixelY, TEXT_COLOR);
    
    // Avanzar cursor
    cursorX++;
}

void ncNewline() {
    initConsole();
    
    // Limpiar el resto de la línea
    while (cursorX < maxCols) {
        uint64_t pixelX = cursorX * getFontWidth();
        uint64_t pixelY = cursorY * getFontHeight();
        
        // Dibujar espacio negro
        for (int y = 0; y < getFontHeight(); y++) {
            for (int x = 0; x < getFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
        cursorX++;
    }
    
    // Ir a la siguiente línea
    cursorX = 0;
    cursorY++;
    
    if (cursorY >= maxRows) {
        cursorY = 0;
    }
}

void ncPrintDec(uint64_t value) {
    ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value) {
    ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value) {
    ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear() {
    initConsole();
    
    // Limpiar toda la pantalla
    for (uint16_t row = 0; row < maxRows; row++) {
        for (uint16_t col = 0; col < maxCols; col++) {
            uint64_t pixelX = col * getFontWidth();
            uint64_t pixelY = row * getFontHeight();
            
            for (int y = 0; y < getFontHeight(); y++) {
                for (int x = 0; x < getFontWidth(); x++) {
                    putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
                }
            }
        }
    }
    
    cursorX = 0;
    cursorY = 0;
}

void ncBackspace() {
    initConsole();
    
    if (cursorX > 0) {
        cursorX--;
        
        uint64_t pixelX = cursorX * getFontWidth();
        uint64_t pixelY = cursorY * getFontHeight();
        
        // Borrar dibujando negro
        for (int y = 0; y < getFontHeight(); y++) {
            for (int x = 0; x < getFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
    } else if (cursorY > 0) {
        cursorY--;
        cursorX = maxCols - 1;
        
        uint64_t pixelX = cursorX * getFontWidth();
        uint64_t pixelY = cursorY * getFontHeight();
        
        for (int y = 0; y < getFontHeight(); y++) {
            for (int x = 0; x < getFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
    }
}

void ncTab() {
    initConsole();
    
    do {
        ncPrintChar(' ');
    } while (cursorX % 4 != 0);
}

uint16_t ncGetCursorX(void) {
    return cursorX;
}

uint16_t ncGetCursorY(void) {
    return cursorY;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    do {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    *p = 0;

    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

