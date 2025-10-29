// #include "stdio.h"
// #include "string.h"

// int consoleMain(void);


// void execute_command(const char *cmd) {
//     if (strcmp(cmd, "help") == 0) {
//         puts("Comandos disponibles:\n");
//         puts("  help   - Muestra esta ayuda\n");
//         puts("  info   - Informacion del sistema\n");
//     } 
//     else if (strcmp(cmd, "info") == 0) {
//         puts("Shell ejecutandose en USERLAND (Ring 3)\n");
//         puts("Sistema: x64BareBones\n");
//         puts("Usando syscalls para I/O\n");
//     }
//     else if (cmd[0] != '\0') {
//         puts("Comando no reconocido: ");
//         puts(cmd);
//         puts("\nEscribe 'help' para ayuda.\n");
//     }
// }

// int consoleMain(void) {
//     char buffer[128];
//     int index = 0;
    
//     puts("\n========================================\n");
//     puts("  Shell en Userland - Ring 3\n");
//     puts("========================================\n\n");
//     puts("> ");
    
//     while (1) {
//         int c = getchar();
        
//         if (c < 0) continue;
        
//         if (c == '\n') {
//             buffer[index] = '\0';
//             execute_command(buffer);
//             index = 0;
//             puts("> ");
//         }
//         else if (c == '\b') {
//             if (index > 0) {
//                 index--;
//             }
//         }
//         else if (index < 127) {
//             buffer[index++] = c;
//         }
//     }
    
//     return 0;
// }