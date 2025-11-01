#include "stdio.h"
#include "syscalls.h"
#include "time.h"

void putchar(char c) {
    write(1, &c, 1);
}

void puts(const char *s) {
    int len = 0;
    while (s[len]) len++;
    write(1, s, len);
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
    char buffer[16];  // suficiente para números de 32 bits
    int i = 0;

    // Caso especial: 0
    if (num == 0) {
        putchar('0');
        return;
    }

    // Convertir a caracteres (al revés)
    while (num > 0 && i < 15) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Imprimir en orden correcto
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

// Función para imprimir fecha y hora formateada
void print_date(void) {
    rtc_datetime_t datetime;
    
    if (get_datetime(&datetime) == 0) {
        const char *days[] = {"", "Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
        const char *months[] = {"", "Ene", "Feb", "Mar", "Abr", "May", "Jun",
                               "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
        
        puts("Fecha y hora: ");
        
        // Mostrar día de la semana
        if (datetime.date.weekday >= 1 && datetime.date.weekday <= 7) {
            puts(days[datetime.date.weekday]);
            puts(" ");
        }
        
        // Mostrar día del mes
        puts_number(datetime.date.day);
        puts(" ");
        
        // Mostrar mes
        if (datetime.date.month >= 1 && datetime.date.month <= 12) {
            puts(months[datetime.date.month]);
        }
        puts(" ");
        
        // Mostrar año
        puts_number(datetime.date.year);
        puts(" - ");
        
        // Mostrar hora (HH:MM:SS)
        if (datetime.time.hours < 10) putchar('0');
        puts_number(datetime.time.hours);
        putchar(':');
        
        if (datetime.time.minutes < 10) putchar('0');
        puts_number(datetime.time.minutes);
        putchar(':');
        
        if (datetime.time.seconds < 10) putchar('0');
        puts_number(datetime.time.seconds);
        puts("\n");
    } else {
        puts("Error obteniendo la fecha\n");
    }
}