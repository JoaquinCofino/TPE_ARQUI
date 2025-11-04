/* sampleCodeModule.c */
#include "stdio.h"
#include "string.h"
#include "sonidos.h"
#include "tron.h"
#include "syscalls.h"
#define CONSOLE_START_X 10
#define CONSOLE_START_Y 10
#define MAX_BUFFER 256

// Declaración forward
int consoleMain(void);
void print_shell_header(void);
void test_division_by_zero(void);
void test_invalid_opcode_exception(void);

void print_shell_header(void) {
    puts("========================================");
    puts("  Shell con Nuevas Syscalls");
    puts("========================================");
    puts("Escribe 'help' para ver comandos");
    puts("Ctrl+R = Registros (incluso en TRON!)");
}

// Función para testear división por cero
void test_division_by_zero(void) {
    puts("=== TEST: Division por Cero ===");
    puts("ADVERTENCIA: Esto causara una excepcion del sistema!");
    puts("El sistema mostrara informacion detallada y luego continuara.");
    puts("Presiona 'y' para continuar o cualquier otra tecla para cancelar: ");
    
    int c = getchar();
    putchar('\n');
    
    if (c == 'y' || c == 'Y') {
        puts("Ejecutando division por cero...");
        puts("Preparando para la operacion...");
        
        // Operación directa que causará división por cero
        volatile int a = 10;
        volatile int b = 0;
        volatile int result = a / b;  // Esto causará excepción #0
        
        // Si llegamos aquí, significa que la excepción fue manejada correctamente
        puts("========================================");
        puts("¡EXITO! El sistema manejo la excepcion correctamente");
        puts("La division por cero fue detectada y el sistema continuo");
        puts("funcionando normalmente.");
        puts("========================================");
    } else {
        puts("Test cancelado.");
    }
}

// Función para testear excepción de opcode inválido
void test_invalid_opcode_exception(void) {
    puts("=== TEST: Excepcion de Opcode Invalido ===");
    puts("ADVERTENCIA: Esto causara una excepcion del sistema!");
    puts("El sistema mostrara informacion detallada y luego continuara.");
    puts("Presiona 'y' para continuar o cualquier otra tecla para cancelar: ");
    
    int c = getchar();
    putchar('\n');
    
    if (c == 'y' || c == 'Y') {
        puts("Ejecutando instruccion con opcode invalido...");
        puts("Llamando a funcion en ASM que contiene opcode 0x0F 0xFF");
        
        // Llamar a la función que contiene el opcode inválido
        trigger_invalid_opcode();  // Causará excepción #6
        
        // Si llegamos aquí, significa que la excepción fue manejada correctamente
        puts("========================================");
        puts("¡EXITO! El sistema manejo la excepcion correctamente");
        puts("El opcode invalido fue detectado y el sistema continuo");
        puts("funcionando normalmente.");
        puts("========================================");
    } else {
        puts("Test cancelado.");
    }
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
        puts("  tron     - Iniciar juego Tron");
        puts("  fontinc  - Aumentar escala de fuente");
        puts("  fontdec  - Disminuir escala de fuente");
        puts("  bench    - Ejecutar benchmarks del sistema");
        puts("  div0     - Test de excepcion division por 0");
        puts("  invop    - Test de excepcion opcode invalido");
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
    else if(strcmp(cmd, "div0") == 0)
    {
        test_division_by_zero();
    }
    else if(strcmp(cmd, "invop") == 0)
    {
        test_invalid_opcode_exception();
    }
    else if(strcmp(cmd, "audio") == 0) {
    puts("========================================");
    puts("       Seleccion de Sonido");
    puts("========================================");
    puts("0) Error");
    puts("1) Beep simple");
    puts("2) Star Wars");
    puts("3) Tetris");
    puts("4) Nokia ringtone");
    puts("5) Escala ascendente");
    puts("6) Sirena policial");
    puts("7) Laser");
    puts("8) Explosion");
    puts("9) Exito");
    puts("----------------------------------------");
    puts("Elige una opcion (0-9): ");

    int c = getchar();
    putchar('\n');  // salto de línea visual

    switch (c) {
        case '1':
            puts("Beep simple!");
            play_sound(440, 300);
            break;
        case '2':
            play_star_wars();
            break;
        case '3':
            play_tetris();
            break;
        case '4':
            play_nokia_ringtone();
            break;
        case '5':
            play_scale_up();
            break;
        case '6':
            play_siren();
            break;
        case '7':
            play_laser();
            break;
        case '8':
            play_explosion();
            break;
        case '9':
            play_success_sound();
            break;
        case '0':
            play_error_sound();
            break;
        default:
            puts("Opcion no valida.");
            break;
    }
}
    else if(strcmp(cmd, "tron") ==0){
        puts("Iniciando Tron...");
        puts("(Fuente sera ajustada automaticamente)");
        tron_main();        // <-- corre el juego (debe RETORNAR)
        clear_screen();     // limpia framebuffer
        print_shell_header();
        puts("Fuente restaurada!");
    }
    else if(strcmp(cmd, "fontdec") == 0) {
    decrease_font_scale();
    clear_screen();
    print_shell_header();
    puts("Fuente reducida!");
    }
    else if(strcmp(cmd, "fontinc") == 0) {
    increase_font_scale();
    clear_screen();
    print_shell_header();
    puts("Fuente aumentada!");
    }
    else if (strcmp(cmd, "bench") == 0) {
        benchmark_run_all();
        video_clear();
        print_shell_header();
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
        
        // Ctrl+R ahora se maneja directamente en el kernel - no necesario aquí
        
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
    
    //play_tetris();
    return consoleMain();
}