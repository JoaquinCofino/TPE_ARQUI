#include <stdint.h>
#include <naiveConsole.h>

extern uint8_t read_port(uint16_t num);
extern void write_port(uint16_t port, uint8_t value);

// Estados de las teclas modificadoras
static uint8_t shift_pressed = 0;
static uint8_t altgr_pressed = 0;
static uint8_t caps_lock = 0;
static uint8_t ctrl_pressed = 0;

// Scancodes de las teclas modificadoras
#define LSHIFT_SCANCODE 0x2A
#define RSHIFT_SCANCODE 0x36
#define LALT_SCANCODE   0x38
#define RALT_SCANCODE   0xB8  // AltGr 
#define CAPS_SCANCODE   0x3A
#define LCTRL_SCANCODE  0x1D 

// Tabla básica (sin modificadores) - Teclado Argentino
static unsigned char makeCodeToAscii[128] = {
    0,      // 0x00 - unused
    '\e',   // 0x01 - ESC
    '1',    // 0x02
    '2',    // 0x03
    '3',    // 0x04
    '4',    // 0x05
    '5',    // 0x06
    '6',    // 0x07
    '7',    // 0x08
    '8',    // 0x09
    '9',    // 0x0A
    '0',    // 0x0B
    '\'',   // 0x0C - ' (apostrofe)
    191,    // 0x0D - ¿ (inverted question mark)
    '\b',   // 0x0E - Backspace
    '\t',   // 0x0F - Tab
    'q',    // 0x10
    'w',    // 0x11
    'e',    // 0x12
    'r',    // 0x13
    't',    // 0x14
    'y',    // 0x15
    'u',    // 0x16
    'i',    // 0x17
    'o',    // 0x18
    'p',    // 0x19
    180,    // 0x1A - ´ (acute accent) - dead key en argentino
    '+',    // 0x1B - +
    '\n',   // 0x1C - Enter
    0,      // 0x1D - Left Ctrl
    'a',    // 0x1E
    's',    // 0x1F
    'd',    // 0x20
    'f',    // 0x21
    'g',    // 0x22
    'h',    // 0x23
    'j',    // 0x24
    'k',    // 0x25
    'l',    // 0x26
    241,    // 0x27 - ñ
    '{',    // 0x28 - { (en layout argentino)
    170,    // 0x29 - ª (ordinal a)
    0,      // 0x2A - Left Shift
    '}',    // 0x2B - }
    'z',    // 0x2C
    'x',    // 0x2D
    'c',    // 0x2E
    'v',    // 0x2F
    'b',    // 0x30
    'n',    // 0x31
    'm',    // 0x32
    ',',    // 0x33
    '.',    // 0x34
    '-',    // 0x35 - - (guión)
    0,      // 0x36 - Right Shift
    '*',    // 0x37 - Keypad *
    0,      // 0x38 - Left Alt
    ' ',    // 0x39 - Space
    0,      // 0x3A - Caps Lock
    0,      // 0x3B - F1
    0,      // 0x3C - F2
    0,      // 0x3D - F3
    0,      // 0x3E - F4
    0,      // 0x3F - F5
    0,      // 0x40 - F6
    0,      // 0x41 - F7
    0,      // 0x42 - F8
    0,      // 0x43 - F9
    0,      // 0x44 - F10
    0,      // 0x45 - Num Lock
    0,      // 0x46 - Scroll Lock
    '7',    // 0x47 - Keypad 7
    '8',    // 0x48 - Keypad 8
    '9',    // 0x49 - Keypad 9
    '-',    // 0x4A - Keypad -
    '4',    // 0x4B - Keypad 4
    '5',    // 0x4C - Keypad 5
    '6',    // 0x4D - Keypad 6
    '+',    // 0x4E - Keypad +
    '1',    // 0x4F - Keypad 1
    '2',    // 0x50 - Keypad 2
    '3',    // 0x51 - Keypad 3
    '0',    // 0x52 - Keypad 0
    '.',    // 0x53 - Keypad .
    0,      // 0x54
    0,      // 0x55
    '<',    // 0x56 '<'
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x57-0x60 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x61-0x6A unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x6B-0x74 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x75-0x7E unused
    0                              // 0x7F unused
};

// Tabla con Shift presionado - Teclado Argentino
static unsigned char shiftCodeToAscii[128] = {
    0,      // 0x00 - unused
    '\e',   // 0x01 - ESC
    '!',    // 0x02 - !
    '"',    // 0x03 - "
    '#',    // 0x04 - #
    '$',    // 0x05 - $
    '%',    // 0x06 - %
    '&',    // 0x07 - &
    '/',    // 0x08 - /
    '(',    // 0x09 - (
    ')',    // 0x0A - )
    '=',    // 0x0B - =
    '?',    // 0x0C - ?
    161,    // 0x0D - ¡ (inverted exclamation)
    '\b',   // 0x0E - Backspace
    '\t',   // 0x0F - Tab
    'Q',    // 0x10
    'W',    // 0x11
    'E',    // 0x12
    'R',    // 0x13
    'T',    // 0x14
    'Y',    // 0x15
    'U',    // 0x16
    'I',    // 0x17
    'O',    // 0x18
    'P',    // 0x19
    168,    // 0x1A - ¨ (diaeresis) - dead key
    '*',    // 0x1B - *
    '\n',   // 0x1C - Enter
    0,      // 0x1D - Left Ctrl
    'A',    // 0x1E
    'S',    // 0x1F
    'D',    // 0x20
    'F',    // 0x21
    'G',    // 0x22
    'H',    // 0x23
    'J',    // 0x24
    'K',    // 0x25
    'L',    // 0x26
    209,    // 0x27 - Ñ
    '[',    // 0x28 - [
    186,    // 0x29 - º (ordinal o)
    0,      // 0x2A - Left Shift
    ']',    // 0x2B - ]
    'Z',    // 0x2C
    'X',    // 0x2D
    'C',    // 0x2E
    'V',    // 0x2F
    'B',    // 0x30
    'N',    // 0x31
    'M',    // 0x32
    ';',    // 0x33 - ;
    ':',    // 0x34 - :
    '_',    // 0x35 - _
    0,      // 0x36 - Right Shift
    '*',    // 0x37 - Keypad *
    0,      // 0x38 - Left Alt
    ' ',    // 0x39 - Space
    0,      // 0x3A - Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x3B-0x44 F keys
    0,      // 0x45 - Num Lock
    0,      // 0x46 - Scroll Lock
    '7',    // 0x47 - Keypad 7
    '8',    // 0x48 - Keypad 8
    '9',    // 0x49 - Keypad 9
    '-',    // 0x4A - Keypad -
    '4',    // 0x4B - Keypad 4
    '5',    // 0x4C - Keypad 5
    '6',    // 0x4D - Keypad 6
    '+',    // 0x4E - Keypad +
    '1',    // 0x4F - Keypad 1
    '2',    // 0x50 - Keypad 2
    '3',    // 0x51 - Keypad 3
    '0',    // 0x52 - Keypad 0
    '.',    // 0x53 - Keypad .
    0, 0,   // 0x54-0x55
    '>',    // 0x56 - >
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x57-0x60 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x61-0x6A unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x6B-0x74 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x75-0x7E unused
    0                              // 0x7F unused
};

// Tabla con AltGr presionado - Teclado Argentino
static unsigned char altgrCodeToAscii[128] = {
    0,      // 0x00 - unused
    0,      // 0x01 - ESC
    '|',    // 0x02 - | (pipe)
    '@',    // 0x03 - @
    '#',    // 0x04 - #
    '~',    // 0x05 - ~ (tilde)
    0,      // 0x06 - no AltGr para 6
    0,      // 0x07 - no AltGr para 7
    0,      // 0x08 - no AltGr para 8
    0,      // 0x09 - no AltGr para 9
    0,      // 0x0A - no AltGr para 0
    0,      // 0x0B - no AltGr para 0
    '\\',   // 0x0C - \ (backslash)
    0,      // 0x0D - no AltGr para ¿
    '\b',   // 0x0E - Backspace
    '\t',   // 0x0F - Tab
    '@',    // 0x10 - @ (alternativo en q)
    0,      // 0x11 - no AltGr para w
    164,    // 0x12 - € (euro symbol) - usando código extendido ASCII
    0,      // 0x13 - no AltGr para r
    0,      // 0x14 - no AltGr para t
    0,      // 0x15 - no AltGr para y
    0,      // 0x16 - no AltGr para u
    0,      // 0x17 - no AltGr para i
    0,      // 0x18 - no AltGr para o
    0,      // 0x19 - no AltGr para p
    '`',    // 0x1A - ` (grave accent)
    '[',    // 0x1B - [
    '\n',   // 0x1C - Enter
    0,      // 0x1D - Left Ctrl
    0,      // 0x1E - no AltGr para a
    0,      // 0x1F - no AltGr para s
    0,      // 0x20 - no AltGr para d
    0,      // 0x21 - no AltGr para f
    0,      // 0x22 - no AltGr para g
    0,      // 0x23 - no AltGr para h
    0,      // 0x24 - no AltGr para j
    0,      // 0x25 - no AltGr para k
    0,      // 0x26 - no AltGr para l
    0,      // 0x27 - no AltGr para ñ
    '^',    // 0x28 - ^ (circumflex)
    '\\',   // 0x29 - \ (backslash alternativo)
    0,      // 0x2A - Left Shift
    ']',    // 0x2B - ]
    0,      // 0x2C - no AltGr para z
    0,      // 0x2D - no AltGr para x
    0,      // 0x2E - no AltGr para c
    0,      // 0x2F - no AltGr para v
    0,      // 0x30 - no AltGr para b
    0,      // 0x31 - no AltGr para n
    0,      // 0x32 - no AltGr para m
    0,      // 0x33 - no AltGr para ,
    0,      // 0x34 - no AltGr para .
    0,      // 0x35 - no AltGr para -
    0,      // 0x36 - Right Shift
    '*',    // 0x37 - Keypad *
    0,      // 0x38 - Left Alt
    ' ',    // 0x39 - Space
    0,      // 0x3A - Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x3B-0x44 F keys
    0,      // 0x45 - Num Lock
    0,      // 0x46 - Scroll Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x47-0x50 Keypad
    0, 0, 0, 0, 0, 0,              // 0x51-0x55
    '\\',   // 0x56 - \ (backslash con AltGr)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x57-0x60 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x61-0x6A unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x6B-0x74 unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x75-0x7E unused
    0                              // 0x7F unused
};

#define SCANCODE_BUFFER_SIZE 32
uint8_t scancodeBuffer[SCANCODE_BUFFER_SIZE];
volatile uint8_t bufferHead = 0;
volatile uint8_t bufferTail = 0;

void keyboard_handler() {
    uint8_t scancode = read_port(0x60);

    // === DEBUG INMEDIATO: Ctrl+R ===
    // Detectar Ctrl+R ANTES de procesar otras teclas
    if (ctrl_pressed && scancode == 0x13) {  // 0x13 = scancode de 'R'
        // Llamar directamente a sys_debug_break()
        extern int64_t sys_debug_break(void);
        sys_debug_break();
        return;  
    }

    // Manejar teclas modificadoras inmediatamente
    switch (scancode) {
        case LSHIFT_SCANCODE:
        case RSHIFT_SCANCODE:
            shift_pressed = 1;
            return;
        case LSHIFT_SCANCODE | 0x80:
        case RSHIFT_SCANCODE | 0x80:
            shift_pressed = 0;
            return;
        case LCTRL_SCANCODE: 
            ctrl_pressed = 1;
            return;
        case LCTRL_SCANCODE | 0x80:  
            ctrl_pressed = 0;
            return;
        case LALT_SCANCODE:
            altgr_pressed = 1;  // Tratamos Alt izquierdo como AltGr por simplicidad
            return;
        case LALT_SCANCODE | 0x80:
            altgr_pressed = 0;
            return;
        case CAPS_SCANCODE:
            caps_lock = !caps_lock;  // Toggle Caps Lock
            return;
    }

    // Guardar en buffer circular solo teclas que no son modificadoras
    uint8_t nextHead = (bufferHead + 1) % SCANCODE_BUFFER_SIZE;
    if (nextHead != bufferTail) { // evitar sobreescribir
        scancodeBuffer[bufferHead] = scancode;
        bufferHead = nextHead;
    }
}

void process_keyboard() {
    while (bufferTail != bufferHead) {
        uint8_t sc = scancodeBuffer[bufferTail];
        bufferTail = (bufferTail + 1) % SCANCODE_BUFFER_SIZE;

        // Si el bit más alto está en 1, no imprime nada, es un break code.
        if (sc & 0x80) continue;

        
        // Convertir scancode a ASCII usando la tabla apropiada
        if (sc < 256) {
            unsigned char ascii = 0;
            
            // Determinar qué tabla usar según las teclas modificadoras
            if (altgr_pressed) {
                ascii = altgrCodeToAscii[sc];
            } else if (shift_pressed) {
                ascii = shiftCodeToAscii[sc];
            } else {
                ascii = makeCodeToAscii[sc];
                
                // Aplicar Caps Lock para letras
                if (caps_lock && ascii >= 'a' && ascii <= 'z') {
                    ascii = ascii - 'a' + 'A';
                } else if (caps_lock && ascii >= 'A' && ascii <= 'Z') {
                    ascii = ascii - 'A' + 'a';
                }
            }
            
            // Aplicar Shift + Caps Lock para letras (se cancelan mutuamente)
            if (shift_pressed && caps_lock && ascii >= 'A' && ascii <= 'Z') {
                ascii = ascii - 'A' + 'a';
            } else if (shift_pressed && caps_lock && ascii >= 'a' && ascii <= 'z') {
                ascii = ascii - 'a' + 'A';
            }
            
            if (ascii != 0) {
                // === MANEJAR CTRL + TECLA ===
                if (ctrl_pressed && ascii >= 'a' && ascii <= 'z') {
                    // Ctrl+letra: generar código ASCII 1-26 (Ctrl+A=1, Ctrl+B=2, etc.)
                    ascii = ascii - 'a' + 1;
                } else if (ctrl_pressed && ascii >= 'A' && ascii <= 'Z') {
                    // Ctrl+MAYÚSCULA: también generar código 1-26
                    ascii = ascii - 'A' + 1;
                }
                
                extern void kernel_stdin_push(unsigned char c);
                
                kernel_stdin_push(ascii);
            }
        }
    }
}