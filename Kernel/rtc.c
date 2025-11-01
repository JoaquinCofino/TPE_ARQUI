#include "lib.h"
#include "rtc.h"
#include <stdint.h>

#define RTC_PORT_SELECT 0x70
#define RTC_PORT_DATA   0x71

// Registros RTC
#define RTC_SECONDS     0x00
#define RTC_MINUTES     0x02
#define RTC_HOURS       0x04
#define RTC_STATUS_B    0x0B
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY_OF_MONTH    0x07
#define RTC_MONTH           0x08  
#define RTC_YEAR            0x09

// Función para convertir BCD a binario
static uint8_t bcd_to_binary(uint8_t bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

// Esperar a que RTC esté disponible
static void rtc_wait_ready(void) {
    int timeout = 1000000;
    while ((read_port(RTC_PORT_SELECT) & 0x80) && timeout > 0) {
        timeout--;
    }
}

// Leer un registro específico del RTC
static uint8_t rtc_read_register(uint8_t reg) {
    rtc_wait_ready();
    write_port(RTC_PORT_SELECT, reg);
    return read_port(RTC_PORT_DATA);
}

void rtc_read_hardware_time(rtc_time_t *time) {
    
    uint8_t seconds = rtc_read_register(RTC_SECONDS);
    uint8_t minutes = rtc_read_register(RTC_MINUTES);
    uint8_t hours = rtc_read_register(RTC_HOURS);
    uint8_t status = rtc_read_register(RTC_STATUS_B);
    
    // Verificar si está en formato BCD
    int is_bcd = !(status & 0x04);
    int is_12h = !(status & 0x02);
    
    // Convertir de BCD si es necesario
    if (is_bcd) {
        time->seconds = bcd_to_binary(seconds);
        time->minutes = bcd_to_binary(minutes);
        time->hours = bcd_to_binary(hours & 0x7F);
    } else {
        time->seconds = seconds;
        time->minutes = minutes;
        time->hours = hours & 0x7F;
    }
    
    // Convertir de 12h a 24h si es necesario
    if (is_12h && (hours & 0x80)) {
        time->hours = (time->hours + 12) % 24;
    }
    
    time->ticks = 0;
}

void rtc_read_full_datetime(rtc_datetime_t *datetime) {
    
    // Leer todos los registros de una vez para ser más rápido
    uint8_t seconds = rtc_read_register(RTC_SECONDS);
    uint8_t minutes = rtc_read_register(RTC_MINUTES);
    uint8_t hours = rtc_read_register(RTC_HOURS);
    uint8_t day = rtc_read_register(RTC_DAY_OF_MONTH);
    uint8_t month = rtc_read_register(RTC_MONTH);
    uint8_t year = rtc_read_register(RTC_YEAR);
    uint8_t weekday = rtc_read_register(RTC_DAY_OF_WEEK);
    uint8_t status = rtc_read_register(RTC_STATUS_B);
    
    // Procesar todo junto
    int is_bcd = !(status & 0x04);
    int is_12h = !(status & 0x02);
    
    if (is_bcd) {
        datetime->time.seconds = bcd_to_binary(seconds);
        datetime->time.minutes = bcd_to_binary(minutes);
        datetime->time.hours = bcd_to_binary(hours & 0x7F);
        datetime->date.day = bcd_to_binary(day);
        datetime->date.month = bcd_to_binary(month);
        datetime->date.year = bcd_to_binary(year);
        datetime->date.weekday = bcd_to_binary(weekday);
    } else {
        datetime->time.seconds = seconds;
        datetime->time.minutes = minutes;
        datetime->time.hours = hours & 0x7F;
        datetime->date.day = day;
        datetime->date.month = month;
        datetime->date.year = year;
        datetime->date.weekday = weekday;
    }
    
    // Convertir de 12h a 24h si es necesario
    if (is_12h && (hours & 0x80)) {
        datetime->time.hours = (datetime->time.hours + 12) % 24;
    }
    
    // El año del RTC es solo 2 dígitos
    datetime->date.year += 2000;
    datetime->time.ticks = 0;
}