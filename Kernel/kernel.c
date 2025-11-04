#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "videoDriver.h"
#include "int_keyboard.h"
#include <idtLoader.h>
#include "time.h"
#include "syscalls.h"
#include "interrupts.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
extern void initIRQHandlers(void);

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
    memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
    return (void*)(
        (uint64_t)&endOfKernel
        + PageSize * 8
        - sizeof(uint64_t)
    );
}

void * initializeKernelBinary()
{
    char buffer[10];

    ncPrint("[x64BareBones]");
    ncNewline();

    ncPrint("CPU Vendor:");
    ncPrint(cpuVendor(buffer));
    ncNewline();

    ncPrint("[Loading modules]");
    ncNewline();
    void * moduleAddresses[] = {
        sampleCodeModuleAddress,
        sampleDataModuleAddress
    };

    loadModules(&endOfKernelBinary, moduleAddresses);
    ncPrint("[Done]");
    ncNewline();
    ncNewline();

    ncPrint("[Initializing kernel's binary]");
    ncNewline();

    clearBSS(&bss, &endOfKernel - &bss);

    ncPrint("  text: 0x");
    ncPrintHex((uint64_t)&text);
    ncNewline();
    ncPrint("  rodata: 0x");
    ncPrintHex((uint64_t)&rodata);
    ncNewline();
    ncPrint("  data: 0x");
    ncPrintHex((uint64_t)&data);
    ncNewline();
    ncPrint("  bss: 0x");
    ncPrintHex((uint64_t)&bss);
    ncNewline();

    ncPrint("[Done]");
    ncNewline();
    ncNewline();
    
    return getStackBase();
}

int main(){
    ncPrint("[Initializing Exception Handlers]");
    ncNewline();
    initIRQHandlers();
    ncPrint("  IRQ Handlers initialized");
    ncNewline();
    
    ncPrint("[Loading IDT]");
    ncNewline();
    load_idt();
    ncPrint("  IDT loaded with exception handlers");
    ncNewline();
    
    ncPrint("[Enabling Interrupts]");
    ncNewline();
    _sti();
    ncPrint("  Interrupts enabled");
    ncNewline();
    
    ncPrint("[System Ready]");
    ncNewline();
    ncClear();
    init_timer();
    ((EntryPoint)sampleCodeModuleAddress)();
    while(1) _hlt();
    return 0;
}