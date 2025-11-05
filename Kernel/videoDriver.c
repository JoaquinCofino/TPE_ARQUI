#include "./include/videoDriver.h"
#include "./include/font.h"
#include <stdint.h>

struct vbe_mode_info_structure {
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;
    
    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;
    
    uint32_t framebuffer;
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;
    uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

// Guardamos numerador y denominador por separado
static uint8_t fontScaleNum = 10;    // Numerador (empieza en 10)
static uint8_t fontScaleDen = 10;    // Denominador (siempre 10)

#define MIN_FONT_SCALE_NUM 5    // Mínimo: 5/10 = 0.5x
#define MAX_FONT_SCALE_NUM 50   // Máximo: 50/10 = 5.0x
#define FONT_SCALE_STEP 1       // Incremento: cada paso suma 0.1x

void setFontScale(uint8_t scale) {
    // Para compatibilidad: asume que scale es el numerador con denominador 10
    if (scale >= MIN_FONT_SCALE_NUM && scale <= MAX_FONT_SCALE_NUM) {
        fontScaleNum = scale;
    }
}

uint8_t getFontScale(void) {
    // Devuelve el numerador (para compatibilidad)
    return fontScaleNum;
}

void increaseFontScale(void) {
    if (fontScaleNum < MAX_FONT_SCALE_NUM) {
        fontScaleNum += FONT_SCALE_STEP;
    }
}

void decreaseFontScale(void) {
    if (fontScaleNum > MIN_FONT_SCALE_NUM) {
        fontScaleNum -= FONT_SCALE_STEP;
    }
}

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint16_t w = VBE_mode_info->width;
    uint16_t h = VBE_mode_info->height;
    if (x >= w || y >= h) return;

    uint8_t *framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint32_t bytesPerPixel = (VBE_mode_info->bpp) / 8;
    uint64_t offset = (x * bytesPerPixel) + (y * VBE_mode_info->pitch);

    for (uint32_t i = 0; i < bytesPerPixel; i++) {
        framebuffer[offset + i] = (hexColor >> (8 * i)) & 0xFF;
    }
}



uint32_t getPixel(uint32_t x, uint32_t y) {
    // Verificar límites
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height) {
        return 0x000000;  // Negro si está fuera de rango
    }
    
    // Obtener el framebuffer y calcular posición
    uint8_t *framebuffer = (uint8_t *)(uintptr_t)VBE_mode_info->framebuffer;
    uint8_t pixelWidth = VBE_mode_info->bpp / 8;  // bytes por pixel
    uint64_t offset = (x * pixelWidth) + (y * VBE_mode_info->pitch);
    
    // Leer según profundidad de color
    if (pixelWidth == 3) {
        // RGB/BGR 24 bits - leer 3 bytes
        uint8_t b = framebuffer[offset];
        uint8_t g = framebuffer[offset + 1];
        uint8_t r = framebuffer[offset + 2];
        // Retornar en formato 0xRRGGBB
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    } 
    else if (pixelWidth == 4) {
        // RGBA/BGRA 32 bits - leer 4 bytes como uint32_t
        uint32_t *pixel = (uint32_t *)(framebuffer + offset);
        return *pixel & 0x00FFFFFF;  // Máscara para ignorar alpha
    }
    
    // Fallback si no es 24 o 32 bits
    return 0x000000;
}

// drawChar CON escalado fraccionario
void drawChar(char c, uint64_t x, uint64_t y, uint32_t color) {
    unsigned char index = (unsigned char)c;
    const uint8_t *glyph = FuenteTPE_16_bmp[index];

    uint8_t fontW = FuenteTPE_16_inf.width;
    uint8_t fontH = FuenteTPE_16_inf.height;

    // Obtener color de fondo actual
    extern uint32_t getCurrentBackgroundColor(void);
    uint32_t backgroundColor = getCurrentBackgroundColor();

    // Calcular tamaño escalado usando aritmética entera
    // scaledW = (fontW * fontScaleNum) / fontScaleDen
    uint16_t scaledW = ((uint16_t)fontW * fontScaleNum) / fontScaleDen;
    uint16_t scaledH = ((uint16_t)fontH * fontScaleNum) / fontScaleDen;

    // Dibujar con escalado fraccionario
    for (uint16_t sy = 0; sy < scaledH; sy++) {
        // Calcular qué fila de la fuente original corresponde a sy
        uint8_t srcRow = (sy * fontScaleDen) / fontScaleNum;
        if (srcRow >= fontH) srcRow = fontH - 1;
        
        uint8_t bits = glyph[srcRow];
        
        for (uint16_t sx = 0; sx < scaledW; sx++) {
            // Calcular qué columna de la fuente original corresponde a sx
            uint8_t srcCol = (sx * fontScaleDen) / fontScaleNum;
            if (srcCol >= fontW) srcCol = fontW - 1;
            
            // Determinar el color usar backgroundColor dinámico para el fondo
            uint32_t pixelColor = (bits & (1 << srcCol)) ? color : backgroundColor;
            putPixel(pixelColor, x + sx, y + sy);
        }
    }
}

uint8_t getFontWidth(void) {
    return FuenteTPE_16_inf.width;  // SIN escala
}

uint8_t getFontHeight(void) {
    return FuenteTPE_16_inf.height;  // SIN escala
}
uint8_t getScaledFontWidth(void) {
    return ((uint16_t)FuenteTPE_16_inf.width * fontScaleNum) / fontScaleDen;
}

uint8_t getScaledFontHeight(void) {
    return ((uint16_t)FuenteTPE_16_inf.height * fontScaleNum) / fontScaleDen;
}

uint16_t getScreenWidth(void) {
    return VBE_mode_info->width;
}

uint16_t getScreenHeight(void) {
    return VBE_mode_info->height;
}

void videoClear(void) {
    uint16_t w = getScreenWidth();
    uint16_t h = getScreenHeight();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            putPixel(0x000000, x, y);
        }
    }
}

uint8_t getbpp(void) {
    return VBE_mode_info->bpp;
}

static void kmemset(void *dst, int val, uint32_t n) {
    uint8_t *p = (uint8_t *)dst;
    for (uint32_t i = 0; i < n; i++) p[i] = (uint8_t)val;
}

static void kmemmove(void *dst, const void *src, uint32_t n) {
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    if (d < s) {
        for (uint32_t i = 0; i < n; i++) d[i] = s[i];
    } else if (d > s) {
        for (uint32_t i = n; i > 0; ) { i--; d[i] = s[i]; }
    }
}

/* Desplaza la pantalla 'lines' líneas hacia arriba y limpia el área inferior */
void scrollUpLines(uint32_t lines) {
    if (lines == 0) return;
    uint8_t fontH = getScaledFontHeight(); 
    uint32_t scrollPixels = lines * (uint32_t)fontH;

    uint8_t *framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint32_t rowBytes = VBE_mode_info->pitch;
    uint32_t screenH = VBE_mode_info->height;

    if (scrollPixels >= screenH) {
        kmemset(framebuffer, 0, rowBytes * screenH);
        return;
    }

    uint32_t srcOffset = scrollPixels * rowBytes;
    uint32_t bytesToMove = (screenH - scrollPixels) * rowBytes;

    kmemmove(framebuffer, framebuffer + srcOffset, bytesToMove);
    kmemset(framebuffer + bytesToMove, 0, scrollPixels * rowBytes);
}

void *getFramebuffer(void) {
    return (void *)(uintptr_t)VBE_mode_info->framebuffer;
}

uint32_t getFramebufferPitch(void) {
    return VBE_mode_info->pitch;
}