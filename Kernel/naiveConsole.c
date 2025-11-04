#include <stdint.h>

#include "include/naiveConsole.h"
#include "include/videoDriver.h"

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };

// Variables para el cursor de texto (posición en caracteres)
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;

// Dimensiones de la consola en caracteres
static uint16_t maxCols = 0;
static uint16_t maxRows = 0;

// Cache del font scale para detectar cambios
static uint8_t lastFontScale = 0;

// Color del texto
#define TEXT_COLOR 0xFFFFFF
#define BACKGROUND_COLOR 0x000000

// ============================================================
// Inicializar/Reinicializar las dimensiones de la consola
// ============================================================
static void updateConsole(void) {
    maxCols = getScreenWidth() / getScaledFontWidth();
    maxRows = getScreenHeight() / getScaledFontHeight();
    lastFontScale = getFontScale();
    
    // Ajustar cursor si está fuera de límites
    if (cursorX >= maxCols) cursorX = 0;
    if (cursorY >= maxRows) cursorY = 0;
}

static void checkConsole(void) {
    static int initialized = 0;
    
    // Primera inicialización
    if (!initialized) {
        updateConsole();
        initialized = 1;
        return;
    }
    
    // Detectar si cambió el font scale
    if (getFontScale() != lastFontScale) {
        updateConsole();
        // Limpiar pantalla automáticamente cuando cambia el scale
        videoClear();
        cursorX = 0;
        cursorY = 0;
    }
}
// ============================================================

void ncPrint(const char * string) {
    int i;
    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}

void ncPrintChar(char character) {
    checkConsole();
    
    unsigned char uc = (unsigned char)character;
    
    // Verificar si necesitamos hacer scroll
    if (cursorY >= maxRows) {
        scrollUpLines(1);
        cursorY = maxRows - 1;
        cursorX = 0;
    }
    
    // Verificar si llegamos al final de la línea
    if (cursorX >= maxCols) {
        ncNewline();
    }
    
    // Calcular posición en píxeles usando funciones ESCALADAS
    uint64_t pixelX = cursorX * getScaledFontWidth();
    uint64_t pixelY = cursorY * getScaledFontHeight();
    
    // Dibujar el carácter (drawChar ya maneja el escalado internamente)
    drawChar(uc, pixelX, pixelY, TEXT_COLOR);
    
    // Avanzar cursor
    cursorX++;
}

void ncNewline() {
    checkConsole();
    
    // Limpiar el resto de la línea
    while (cursorX < maxCols) {
        uint64_t pixelX = cursorX * getScaledFontWidth();
        uint64_t pixelY = cursorY * getScaledFontHeight();
        
        // Dibujar espacio negro
        for (int y = 0; y < getScaledFontHeight(); y++) {
            for (int x = 0; x < getScaledFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
        cursorX++;
    }
    
    // Ir a la siguiente línea
    cursorX = 0;
    cursorY++;
    
    if (cursorY >= maxRows) {
        scrollUpLines(1);
        cursorY = maxRows - 1;
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
    checkConsole();
    
    // Limpiar toda la pantalla
    for (uint16_t row = 0; row < maxRows; row++) {
        for (uint16_t col = 0; col < maxCols; col++) {
            uint64_t pixelX = col * getScaledFontWidth();
            uint64_t pixelY = row * getScaledFontHeight();
            
            for (int y = 0; y < getScaledFontHeight(); y++) {
                for (int x = 0; x < getScaledFontWidth(); x++) {
                    putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
                }
            }
        }
    }
    
    cursorX = 0;
    cursorY = 0;
}

void ncBackspace() {
    checkConsole();
    
    if (cursorX > 0) {
        cursorX--;
        
        uint64_t pixelX = cursorX * getScaledFontWidth();
        uint64_t pixelY = cursorY * getScaledFontHeight();
        
        // Borrar dibujando negro
        for (int y = 0; y < getScaledFontHeight(); y++) {
            for (int x = 0; x < getScaledFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
    } else if (cursorY > 0) {
        cursorY--;
        cursorX = maxCols - 1;
        
        uint64_t pixelX = cursorX * getScaledFontWidth();
        uint64_t pixelY = cursorY * getScaledFontHeight();
        
        for (int y = 0; y < getScaledFontHeight(); y++) {
            for (int x = 0; x < getScaledFontWidth(); x++) {
                putPixel(BACKGROUND_COLOR, pixelX + x, pixelY + y);
            }
        }
    }
}

void ncTab() {
    checkConsole();
    
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