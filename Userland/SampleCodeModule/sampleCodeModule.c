/* sampleCodeModule.c */
#include "stdio.h"
#include "string.h"
#define CONSOLE_START_X 10
#define CONSOLE_START_Y 10
#define MAX_BUFFER 128

// Declaración forward
int consoleMain(void);


void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        puts("Comandos disponibles:\n");
        puts("  help   - Muestra esta ayuda\n");
        puts("  info   - Informacion del sistema\n");
        puts("  clear  - Limpia la pantalla\n");
        puts("  date   - Obtener Fecha y Hora\n");
    } 
    else if (strcmp(cmd, "info") == 0) {
        puts("Shell ejecutandose en USERLAND (Ring 3)\n");
        puts("Sistema: x64BareBones\n");
        puts("Usando syscalls para I/O\n");
		puts("FALTA COMPLETAR DINAMICAMENTE!");
		puts("\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        puts("HAY QUE IMPLEMENTARLO!");
		puts("\n");
    }
    else if (strcmp(cmd, "date") == 0) {
        print_date();
    }

    else if (cmd[0] != '\0') {
        puts("Comando no reconocido: '");
        puts(cmd);
        puts("'\n");
        puts("Escribe 'help' para ayuda.\n");
		
    }
}

int consoleMain(void) {
    char buffer[128];
    int index = 0;
    
    // Banner inicial con saltos de línea para bajar
    puts("========================================\n");
    puts("  Shell\n");
    puts("========================================\n");
    puts("Escribe 'help' para ver comandos\n\n");
    puts("> ");
    
    while (1) {
        int c = getchar();
        
        if (c < 0) continue;
        
        if (c == '\n') {
            putchar('\n');  // Nueva línea después del enter
            buffer[index] = '\0';
            execute_command(buffer);
            index = 0;
            puts("> ");
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
    return consoleMain();
}