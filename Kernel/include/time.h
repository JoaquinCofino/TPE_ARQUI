#ifndef _TIME_H_
#define _TIME_H_

#include "rtc.h"

// Funciones básicas de tiempo
void init_timer(void);
void timer_handler(void);
void get_current_time(rtc_time_t *time);

// Variables para el timer
extern unsigned int current_hours;
extern unsigned int current_minutes; 
extern unsigned int current_seconds;
extern unsigned int ticks;

#endif