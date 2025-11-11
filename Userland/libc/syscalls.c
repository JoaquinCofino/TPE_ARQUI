#include "../include/syscalls.h"

int64_t write(int fd, const char *buf, uint64_t count) {
    return syscall_write(fd, buf, count);
}

int64_t read(int fd, char *buf, uint64_t count) {
    return syscall_read(fd, buf, count);
}

int64_t read_nb(int fd, char *buf, uint64_t count) {
    return syscall_read_nb(fd, buf, count); 
}

int64_t get_time(rtc_time_t *time_ptr) {
    return syscall_get_time(time_ptr);
}

int64_t get_datetime(rtc_datetime_t *datetime_ptr) {
    return syscall_get_datetime(datetime_ptr);
}

int64_t get_registers(cpu_registers_t *regs) {
    return syscall_get_registers(regs);
}

int64_t get_video_data(video_info_t *video_info) {
    return syscall_get_video_data(video_info);
}

int64_t video_clear(void) {
    return syscall_video_clear();
}

int64_t video_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    return sys_put_pixel(x, y, color);
}

int64_t video_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    return syscall_video_draw_rect(x, y, w, h, color);
}

int64_t play_sound(uint32_t freq, uint32_t dur_ms) {
    return syscall_play_sound(freq, dur_ms);
}

int64_t increase_font_scale(void) {
    return syscall_increase_font_scale();
}

int64_t decrease_font_scale(void) {
    return syscall_decrease_font_scale();
}

int64_t debug_break(void) {
    return syscall_debug_break();
}

void font_save_state(void) {
    syscall_font_save_state();
}

void font_restore_state(void) {
    syscall_font_restore_state();
}

int64_t getFontWidth(void) {
    return syscall_get_font_width();
}

int64_t getFontHeight(void) {
    return syscall_get_font_height();
}

int64_t set_text_color(uint32_t color) {
    return syscall_set_text_color(color);
}

int64_t set_background_color(uint32_t color) {
    return syscall_set_background_color(color);
}

int64_t get_ticks(void) {
    return syscall_get_ticks();
}

extern int64_t sys_video_getpixel(uint64_t x, uint64_t y);

uint32_t video_getpixel(uint32_t x, uint32_t y) {
    return (uint32_t)sys_video_getpixel(x, y);
}

void set_cursor_position(int x, int y) {
    syscall_set_cursor_position(x, y);
}

int64_t get_userland_registers(cpu_registers_t *regs) {
    return syscall_get_userland_registers(regs);
}