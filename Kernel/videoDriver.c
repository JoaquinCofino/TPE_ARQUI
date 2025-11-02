#include "videoDriver.h"
#include "font.h"

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
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void drawChar(char c, uint64_t x, uint64_t y, uint32_t color) {
    // Validar índice del carácter
    unsigned char index = (unsigned char)c;
    if (index >= FuenteTPE_16_inf.nChars) {
        index = '?';  // Carácter por defecto si está fuera de rango
    }
    
    // Obtener el glyph del bitmap font
    const unsigned char *glyph = FuenteTPE_16_bmp[index];
    
    // Dibujar cada fila del carácter
    for (int row = 0; row < FuenteTPE_16_inf.height; row++) {
        uint8_t bits = glyph[row];
        
        // Dibujar cada pixel de la fila
        for (int col = 0; col < FuenteTPE_16_inf.width; col++) {
            // 🔄 INVERTIR: leer del bit menos significativo al más significativo
            if (bits & (1 << col)) {
                putPixel(color, x + col, y + row);
            }
        }
    }
}

void drawString(const char *s, uint64_t x, uint64_t y, uint32_t color) {
    uint64_t currentX = x;
    
    while (*s) {
        drawChar(*s, currentX, y, color);
        currentX += FuenteTPE_16_inf.width;  // Avanzar al siguiente carácter
        s++;
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

#define SCREEN_WIDTH getScreenWidth()
#define SCREEN_HEIGHT getScreenHeight()

void videoClear(void) {
    // Llenar toda la pantalla de negro
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putPixel(0x000000, x, y);  // Negro
        }
    }
}

uint8_t getbpp(void)
{
    return VBE_mode_info->bpp;
}