
#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

void setup_IDT_entry(int index, uint64_t offset);
void load_idt();
void int_20();
void process_keyboard();
void _syscall_handler(void);


#endif // _IDTLOADER_H_
