#include "syscalls.h"

extern int64_t syscall_write(int fd, const char *buf, uint64_t count);
extern int64_t syscall_read(int fd, char *buf, uint64_t count);
extern int64_t syscall_get_time(void *time_ptr);
extern int64_t syscall_get_datetime(void *datetime_ptr);
extern int64_t syscall_get_registers(void *regs);
extern int64_t syscall_get_video_data(void *video_info);
extern int64_t syscall_video_clear(void);
extern int64_t syscall_video_putpixel(uint64_t x, uint64_t y, uint64_t color);
extern int64_t syscall_video_draw_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t color);
extern int64_t syscall_play_sound(uint64_t freq, uint64_t duration_ms);

int64_t write(int fd, const char *buf, uint64_t count) {
    return syscall_write(fd, buf, count);
}

int64_t read(int fd, char *buf, uint64_t count) {
    return syscall_read(fd, buf, count);
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
    return syscall_video_putpixel(x, y, color);
}

int64_t video_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    return syscall_video_draw_rect(x, y, w, h, color);
}

int64_t play_sound(uint32_t freq, uint32_t dur_ms) {
    return syscall_play_sound(freq, dur_ms);
}