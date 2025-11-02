#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)
#pragma pack (1)

typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)

DESCR_INT * idt = (DESCR_INT *) 0;

void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  
  // Syscalls necesitan DPL=3 para ser llamadas desde Ring 3 (userland)
  if (index == 0x80) {
    idt[index].access = 0xEE;  // Present=1, DPL=3, Type=1110 (interrupt gate)
  } else {
    idt[index].access = ACS_INT;  // DPL=0 para interrupciones del kernel
  }
  
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}

void load_idt() {
  _cli();
  setup_IDT_entry(0x20, (uint64_t)&_irq00Handler);
  setup_IDT_entry(0x21, (uint64_t)&_irq01Handler);
  setup_IDT_entry(0x00, (uint64_t)&_exception0Handler);
  setup_IDT_entry(0x80, (uint64_t)&_syscall_handler);  // Syscalls desde userland

  picMasterMask(0xFC); 
  picSlaveMask(0xFF);
  _sti();
}