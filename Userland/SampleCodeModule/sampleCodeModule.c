/* sampleCodeModule.c */
#include "stdio.h"
#include "string.h"
#include "sound_events.h"
#define CONSOLE_START_X 10
#define CONSOLE_START_Y 10
#define MAX_BUFFER 256

// Declaración forward
int consoleMain(void);
void print_shell_header(void);

void print_shell_header(void) {
    puts("========================================");
    puts("  Shell con Nuevas Syscalls");
    puts("========================================");
    puts("Escribe 'help' para ver comandos");
}

void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        puts("Comandos disponibles:");
        puts("  help     - Muestra esta ayuda");
        puts("  info     - Informacion del sistema");
        puts("  date     - Obtener Fecha y Hora");
        puts("  clear    - Limpiar la pantalla");
        puts("  regs     - Mostrar registros del CPU");
        puts("  video    - Informacion de video");
        puts("  audio    - Reproducir sonidos de prueba");
    }
    else if (strcmp(cmd, "info") == 0) {
        puts("Shell ejecutandose en USERLAND (Ring 3)");
        puts("Sistema: x64BareBones");
        puts("Usando syscalls para I/O");
        puts("Scroll automatico cuando se llena la pantalla");
    }
    else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
        print_shell_header();
        puts("Pantalla limpiada manualmente!");
    }
    else if (strcmp(cmd, "date") == 0) {
        print_date();
    }
    else if (strcmp(cmd, "regs") == 0) {
        print_registers();
    }
    else if (strcmp(cmd, "video") == 0) {
        print_video_info();
    }
    else if(strcmp(cmd, "audio") ==0)
    {
        puts("Beep test!");
        play_sound(440, 400);  // A4 - 440 Hz
        play_sound(880, 200);  // 880 Hz
        play_sound(660, 300);
    }
    else if (cmd[0] != '\0') {
        printf("Comando no reconocido: '");
        printf(cmd);
        puts("'");
        puts("Escribe 'help' para ayuda.");
    }
}

int consoleMain(void) {
    char buffer[256];
    int index = 0;
    
    // Banner inicial
    print_shell_header();
    printf("> ");
    
    while (1) {
        int c = getchar();
        
        if (c < 0) continue;
        
        if (c == '\n') {
            putchar('\n');  // Nueva línea después del enter
            buffer[index] = '\0';
            execute_command(buffer);
            index = 0;
            printf("> ");  // Usar printf para prompt sin salto de línea
        }
        else if (c == '\b' || c == 127) {  // Backspace o DEL
            if (index > 0) {
                index--;
                // Secuencia completa de backspace visual
                putchar('\b');  // Retrocede
                putchar(' ');   // Escribe espacio
                putchar('\b');  // Retrocede de nuevo
            }
        }
        else if (c >= 32 && c <= 126) {  // Solo caracteres imprimibles
            if (index < 127) {
                buffer[index++] = c;
                putchar(c);  // Echo del carácter
            }
        }
        else if (c >= 160 && c <= 255) {  // Extended ASCII (ñ, á, etc)
            if (index < 127) {
                buffer[index++] = c;
                putchar(c);
            }
        }
    }
    
    return 0;
}

int main() {
    // puts("Beep test!");
    /*
    play_sound(440, 400);  // A4 - 440 Hz
    play_sound(880, 200);  // 880 Hz
    play_sound(660, 300);*/
    play_tetris();
    return consoleMain();
}