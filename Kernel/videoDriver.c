#include "./include/videoDriver.h"
#include "./include/font.h"
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>


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

void drawChar(char c, uint64_t x, uint64_t y, uint32_t color) {
    unsigned char index = (unsigned char)c;
    if (index >= FuenteTPE_16_inf.nChars) {
        index = '?';
    }
    const uint8_t *glyph = FuenteTPE_16_bmp[index];

    uint8_t fontW = FuenteTPE_16_inf.width;
    uint8_t fontH = FuenteTPE_16_inf.height;

    for (int row = 0; row < fontH; row++) {
        uint8_t bits = glyph[row];
        for (int col = 0; col < fontW; col++) {
            if (bits & (1 << col)) {
                putPixel(color, x + col, y + row);
            } else {
                putPixel(0x000000, x + col, y + row);
            }
        }
    }
}

uint8_t getFontWidth(void) {
    return FuenteTPE_16_inf.width;
}

uint8_t getFontHeight(void) {
    return FuenteTPE_16_inf.height;
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
    uint8_t fontH = getFontHeight();
    uint32_t scrollPixels = lines * (uint32_t)fontH;

    uint8_t *framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint32_t rowBytes = VBE_mode_info->pitch;
    uint32_t screenH = VBE_mode_info->height;

    if (scrollPixels >= screenH) {
        /* limpiar toda la pantalla */
        kmemset(framebuffer, 0, rowBytes * screenH);
        return;
    }

    uint32_t srcOffset = scrollPixels * rowBytes;
    uint32_t bytesToMove = (screenH - scrollPixels) * rowBytes;

    kmemmove(framebuffer, framebuffer + srcOffset, bytesToMove);

    /* limpiar la zona inferior recién liberada */
    kmemset(framebuffer + bytesToMove, 0, scrollPixels * rowBytes);
}


void *getFramebuffer(void) {
    // El campo framebuffer es la dirección física donde empieza el buffer de video
    return (void *)(uintptr_t)VBE_mode_info->framebuffer;
}

uint32_t getFramebufferPitch(void) {
    // Cantidad de bytes que ocupa cada fila de la pantalla
    return VBE_mode_info->pitch;
}