#include "../include/stdio.h"
#include "../include/syscalls.h"
#include "../include/string.h"

int getchar(void) {
    unsigned char c;
    if (read(0, (char*)&c, 1) == 1) {
        return (int)c;
    }
    return -1;
}

int getchar_nb(void) {
    unsigned char c;
    int64_t r = read_nb(0, (char*)&c, 1);
    if (r == 1) return (int)c; 
    return -1;
}

void putchar(char c) {
    write(1, &c, 1);
}

void puts(const char *s) {
    write(1, s, strlen(s));
    putchar('\n');
}

void printf(const char *s) {
    write(1, s, strlen(s));
}

void puts_number(unsigned int num) {
    if (num == 0) {
        putchar('0');
        return;
    }

    char buffer[10];
    int i = 0;

    while (num > 0 && i < 10) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--)
        putchar(buffer[j]);
}

void print_date(void) {
    rtc_datetime_t dt = {0};

    if (get_datetime(&dt) != 0) {
        puts("Error obteniendo la fecha");
        return;
    }

    int dia = dt.date.day;
    int mes = dt.date.month;
    int anio = dt.date.year;
    int hora = dt.time.hours - 3;  
    int min = dt.time.minutes;
    int seg = dt.time.seconds;

    if (hora < 0) {
        hora += 24;
        dia--;
        if (dia <= 0) {
            mes--;
            if (mes <= 0) {
                mes = 12;
                anio--;
            }

            int dias_mes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
            if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
                dias_mes[2] = 29;
            dia = dias_mes[mes];
        }
    }

    if (dia < 10) putchar('0'); 
    puts_number(dia); 
    putchar('/');
    if (mes < 10) putchar('0');
    puts_number(mes); 
    putchar('/');
    
    puts_number(anio);
    printf(" - ");
    if (hora < 10) putchar('0'); 
    puts_number(hora); 
    putchar(':');
    if (min < 10) 
    putchar('0');
    puts_number(min); 
    putchar(':');
    if (seg < 10) putchar('0'); 
    puts_number(seg);
    putchar('\n');
}


void print_registers(void) {
    cpu_registers_t regs;
    
    if (get_userland_registers(&regs) != 0) {
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

void clear_screen(void) {
    video_clear();
}

#define MAX_SIZE 256

int scanf(char *buffer) {
    int idx = 0;
    
    while (1) {
        int c = getchar();  
        
        if (c == '\n') {
            buffer[idx] = '\0';
            putchar('\n');
            return idx;
        }
        
        if (c == '\b' || c == 127) {
            if (idx > 0) {
                idx--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
            continue;
        }
        
        if (idx < MAX_SIZE - 1 && c >= 0 && c <= 255) {  
            buffer[idx++] = (unsigned char)c;
            putchar((char)c);
        }
    }
}