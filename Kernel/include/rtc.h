#ifndef _RTC_H_
#define _RTC_H_

#include <stdint.h>

typedef struct {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int ticks;
} rtc_time_t;

typedef struct {
    unsigned int day;        // 1-31
    unsigned int month;      // 1-12
    unsigned int year;       // 2024
    unsigned int weekday;    // 1-7 (domingo=1)
} rtc_date_t;

typedef struct {
    rtc_time_t time;
    rtc_date_t date;
} rtc_datetime_t;

// Funciones disponibles
void rtc_read_hardware_time(rtc_time_t *time);
void rtc_read_full_datetime(rtc_datetime_t *datetime);  // ← Agregar

#endif