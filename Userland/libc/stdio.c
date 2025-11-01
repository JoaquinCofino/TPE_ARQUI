#include "stdio.h"
#include "syscalls.h"
#include "string.h"

void putchar(char c) {
    write(1, &c, 1);
}

void puts(const char *s) {
    int len = strlen(s);
    write(1, s, len);
    putchar('\n');
}

void printf(const char *s) {
    int len = strlen(s);
    write(1, s, len);
    // No agrega \n automáticamente, a diferencia de puts()
}

int getchar(void) {
    char c;
    if (read(0, &c, 1) == 1) {
        return c;
    }
    return -1;
}

// Función auxiliar para imprimir números
void puts_number(unsigned int num) {
    if (num >= 10) {
        puts_number(num / 10);
    }
    putchar('0' + (num % 10));
}

// Función para imprimir fecha y hora formateada
void print_date(void) {
    rtc_datetime_t datetime = {0};

    // Llamar a la syscall para obtener la fecha y hora
    if (get_datetime(&datetime) != 0) {
        puts("Error obteniendo la fecha");
        return;
    }

    // Día
    if (datetime.date.day < 10) putchar('0');
    puts_number(datetime.date.day);
    putchar('/');

    // Mes
    if (datetime.date.month < 10) putchar('0');
    puts_number(datetime.date.month);
    putchar('/');

    // Año
    puts_number(datetime.date.year);

    putchar(' ');
    // Separador entre fecha y hora
    putchar('-');
    putchar(' ');

    // Hora
    if (datetime.time.hours < 10) putchar('0');
    puts_number(datetime.time.hours);
    putchar(':');

    // Minutos
    if (datetime.time.minutes < 10) putchar('0');
    puts_number(datetime.time.minutes);
    putchar(':');

    // Segundos
    if (datetime.time.seconds < 10) putchar('0');
    puts_number(datetime.time.seconds);

    putchar('\n');
}


// ARREGLADA: Test de flechas y WASD que funciona correctamente
void arrow_test(void) {
    puts("=== TEST DE MOVIMIENTO ===\n");
    puts("Presiona las flechas o WASD para moverte\n");
    puts("Presiona ESC para salir\n");
    puts("Presiona cualquier otra tecla para mostrar estado\n\n");
    
    int should_exit = 0;
    
    while (!should_exit) {
        int c = getchar();
        
        if (c < 0) continue;
        
        // Si presionan ESC, salir
        if (c == 27) {  // ESC
            puts("\nESC presionado - Saliendo del test\n");
            break;
        }
        
        // Mostrar estado actual de las teclas de movimiento
        puts("Estado actual: ");
        
        int any_movement = 0;
        
        // Verificar flechas y WASD
        if (is_key_pressed(KEY_ARROW_UP) || is_key_pressed(KEY_W)) {
            puts("ARRIBA ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_ARROW_DOWN) || is_key_pressed(KEY_S)) {
            puts("ABAJO ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_ARROW_LEFT) || is_key_pressed(KEY_A)) {
            puts("IZQUIERDA ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_ARROW_RIGHT) || is_key_pressed(KEY_D)) {
            puts("DERECHA ");
            any_movement = 1;
        }
        
        // Verificar otras teclas útiles
        if (is_key_pressed(KEY_SPACE)) {
            puts("ESPACIO ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_SHIFT_L) || is_key_pressed(KEY_SHIFT_R)) {
            puts("SHIFT ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_CTRL)) {
            puts("CTRL ");
            any_movement = 1;
        }
        
        if (is_key_pressed(KEY_ALT)) {
            puts("ALT ");
            any_movement = 1;
        }
        
        if (!any_movement) {
            puts("ninguna tecla de movimiento");
        }
        
        puts("\n");
        puts("(Presiona ESC para salir)\n");
    }
}

// Imprimir registros del CPU
void print_registers(void) {
    cpu_registers_t regs;
    
    if (get_registers(&regs) == 0) {
        puts("=== REGISTROS DEL CPU ===\n");
        
        puts("RAX: 0x"); print_hex(regs.rax); puts("\n");
        puts("RBX: 0x"); print_hex(regs.rbx); puts("\n");
        puts("RCX: 0x"); print_hex(regs.rcx); puts("\n");
        puts("RDX: 0x"); print_hex(regs.rdx); puts("\n");
        puts("RSI: 0x"); print_hex(regs.rsi); puts("\n");
        puts("RDI: 0x"); print_hex(regs.rdi); puts("\n");
        puts("RBP: 0x"); print_hex(regs.rbp); puts("\n");
        puts("RSP: 0x"); print_hex(regs.rsp); puts("\n");
        
        puts("R8:  0x"); print_hex(regs.r8); puts("\n");
        puts("R9:  0x"); print_hex(regs.r9); puts("\n");
        puts("R10: 0x"); print_hex(regs.r10); puts("\n");
        puts("R11: 0x"); print_hex(regs.r11); puts("\n");
        puts("R12: 0x"); print_hex(regs.r12); puts("\n");
        puts("R13: 0x"); print_hex(regs.r13); puts("\n");
        puts("R14: 0x"); print_hex(regs.r14); puts("\n");
        puts("R15: 0x"); print_hex(regs.r15); puts("\n");
        
        puts("RIP: 0x"); print_hex(regs.rip); puts("\n");
        puts("RFLAGS: 0x"); print_hex(regs.rflags); puts("\n");
    } else {
        puts("Error obteniendo registros\n");
    }
}

// Imprimir información de video
void print_video_info(void) {
    video_info_t video;
    
    if (get_video_data(&video) == 0) {
        puts("=== INFORMACION DE VIDEO ===\n");
        puts("Ancho: ");
        puts_number(video.width);
        puts(" pixels\n");
        
        puts("Alto: ");
        puts_number(video.height);
        puts(" pixels\n");
        
        puts("Bits por pixel: ");
        puts_number(video.bpp);
        puts("\n");
        
        puts("Framebuffer: 0x");
        print_hex(video.framebuffer);
        puts("\n");
    } else {
        puts("Error obteniendo info de video\n");
    }
}

// Función auxiliar para imprimir números hexadecimales
void print_hex(uint64_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[17];
    int i = 15;
    
    buffer[16] = '\0';
    
    if (num == 0) {
        puts("0");
        return;
    }
    
    while (num > 0 && i >= 0) {
        buffer[i] = hex_chars[num % 16];
        num /= 16;
        i--;
    }
    
    puts(&buffer[i + 1]);
}

// NUEVA: Limpiar pantalla usando la syscall
void clear_screen(void) {
    video_clear();
}