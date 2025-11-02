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

    // Obtener la fecha y hora del RTC
    if (get_datetime(&datetime) != 0) {
        puts("Error obteniendo la fecha");
        return;
    }

    // === Ajustar zona horaria (-3 horas) ===
    int hora = datetime.time.hours - 3;
    int dia = datetime.date.day;
    int mes = datetime.date.month;
    int anio = datetime.date.year;

    // Cantidad de días en cada mes (no considera año bisiesto aún)
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajuste para año bisiesto (febrero con 29 días)
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
        dias_mes[2] = 29;

    // Si la hora se vuelve negativa, retrocedemos un día
    if (hora < 0) {
        hora += 24;
        dia -= 1;

        if (dia < 1) {
            mes -= 1;

            if (mes < 1) {
                mes = 12;
                anio -= 1;
            }

            dia = dias_mes[mes];
        }
    }

    // === Imprimir fecha ===
    if (dia < 10) putchar('0');
    puts_number(dia);
    putchar('/');

    if (mes < 10) putchar('0');
    puts_number(mes);
    putchar('/');

    puts_number(anio);
    putchar(' ');
    putchar('-');
    putchar(' ');

    // === Imprimir hora ajustada ===
    if (hora < 10) putchar('0');
    puts_number(hora);
    putchar(':');

    if (datetime.time.minutes < 10) putchar('0');
    puts_number(datetime.time.minutes);
    putchar(':');

    if (datetime.time.seconds < 10) putchar('0');
    puts_number(datetime.time.seconds);

    putchar('\n');
}

// Imprimir registros del CPU
void print_registers(void) {
    cpu_registers_t regs;
    
    if (get_registers(&regs) != 0) {
        puts("Error obteniendo registros");
        return;
    }

    puts("=== REGISTROS DEL CPU ===");

    printf("RAX: 0x"); print_hex(regs.rax); 
    printf("RBX: 0x"); print_hex(regs.rbx);
    printf("RCX: 0x"); print_hex(regs.rcx);
    printf("RDX: 0x"); print_hex(regs.rdx);
    printf("RSI: 0x"); print_hex(regs.rsi);
    printf("RDI: 0x"); print_hex(regs.rdi);
    printf("RBP: 0x"); print_hex(regs.rbp);
    printf("RSP: 0x"); print_hex(regs.rsp);

    printf("R8:  0x"); print_hex(regs.r8);
    printf("R9:  0x"); print_hex(regs.r9);
    printf("R10: 0x"); print_hex(regs.r10);
    printf("R11: 0x"); print_hex(regs.r11);
    printf("R12: 0x"); print_hex(regs.r12);
    printf("R13: 0x"); print_hex(regs.r13);
    printf("R14: 0x"); print_hex(regs.r14);
    printf("R15: 0x"); print_hex(regs.r15);

    printf("RIP: 0x"); print_hex(regs.rip);
    printf("RFLAGS: 0x"); print_hex(regs.rflags);
}


// Imprimir información de video
void print_video_info(void) {
    video_info_t video;
    
    if (get_video_data(&video) == 0) {
        puts("=== INFORMACION DE VIDEO ===");
        printf("Ancho: ");
        puts_number(video.width);
        puts(" pixels");
        
        printf("Alto: ");
        puts_number(video.height);
        puts(" pixels");

        printf("Bits por pixel:");
        puts_number(video.bpp);
        puts("");
    } else {
        puts("Error obteniendo info de video");
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