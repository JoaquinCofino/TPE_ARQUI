#include "time.h"
#include "rtc.h"
#include "interrupts.h"

#define TIMER_FREQUENCY 18  // PIT estándar

// Variables globales de tiempo
unsigned int current_hours = 0;
unsigned int current_minutes = 0;
unsigned int current_seconds = 0;
unsigned int ticks = 0;
unsigned int system_ticks = 0;

static int time_initialized = 0;

void init_timer(void) {
    rtc_time_t rtc_time;
    
    // Leer tiempo inicial del RTC
    rtc_read_hardware_time(&rtc_time);
    
    current_hours = rtc_time.hours;
    current_minutes = rtc_time.minutes;
    current_seconds = rtc_time.seconds;
    ticks = 0;

    system_ticks = 0;
    
    time_initialized = 1;
}

void timer_handler(void) {
    if (!time_initialized) return;
    
    system_ticks++;
    ticks++;
    
    // Incrementar tiempo cada segundo
    if (ticks >= TIMER_FREQUENCY) {
        ticks = 0;
        current_seconds++;
        
        if (current_seconds >= 60) {
            current_seconds = 0;
            current_minutes++;
            
            if (current_minutes >= 60) {
                current_minutes = 0;
                current_hours++;
                
                if (current_hours >= 24) {
                    current_hours = 0;
                }
            }
        }
    }
}

void get_current_time(rtc_time_t *time) {
    
    _cli();
    time->hours = current_hours;
    time->minutes = current_minutes;
    time->seconds = current_seconds;
    time->ticks = ticks;
    _sti();
}

unsigned int timer_get_ticks(void) {
    return system_ticks;
}