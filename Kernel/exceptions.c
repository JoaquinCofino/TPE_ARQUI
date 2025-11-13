
#include <stdint.h>
#include "include/naiveConsole.h"
#include "include/syscalls.h"

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

extern void haltcpu(void);

static void zero_division(uint64_t rip);
static void invalid_opcode(uint64_t rip);

void exceptionDispatcher(uint64_t exception, uint64_t rip) {
    ncPrint("========================================");
    ncNewline();
    ncPrint("[EXCEPCION CRITICA DEL SISTEMA]");
    ncNewline();
    ncPrint("========================================");
    ncNewline();
    
    if (exception == ZERO_EXCEPTION_ID) {
        zero_division(rip);
    } else if (exception == INVALID_OPCODE_ID) {
        invalid_opcode(rip);
    } else {
        ncPrint("Tipo: Excepcion Desconocida (Exception #");
        ncPrintDec(exception);
        ncPrint(")");
        ncNewline();
        ncPrint("Descripcion: Excepcion no manejada");
        ncNewline();
        
        // Mostrar Instruction Pointer
        ncPrint("Instruction Pointer (RIP): 0x");
        ncPrintHex(rip);
        ncNewline();
    }
    
    // Obtener y mostrar registros usando la syscall
    ncPrint("========================================");
    ncNewline();
    ncPrint("REGISTROS DEL CPU AL MOMENTO DEL ERROR:");
    ncNewline();
    ncPrint("========================================");
    ncNewline();
    
    cpu_registers_t registers;
    if (sys_get_userland_registers(&registers) == 0) {
        ncPrint("RAX: 0x"); ncPrintHex(registers.rax); ncNewline();
        ncPrint("RBX: 0x"); ncPrintHex(registers.rbx); ncNewline();
        ncPrint("RCX: 0x"); ncPrintHex(registers.rcx); ncNewline();
        ncPrint("RDX: 0x"); ncPrintHex(registers.rdx); ncNewline();
        ncPrint("RSI: 0x"); ncPrintHex(registers.rsi); ncNewline();
        ncPrint("RDI: 0x"); ncPrintHex(registers.rdi); ncNewline();
        ncPrint("RBP: 0x"); ncPrintHex(registers.rbp); ncNewline();
        ncPrint("RSP: 0x"); ncPrintHex(registers.rsp); ncNewline();
        ncPrint("R8:  0x"); ncPrintHex(registers.r8);  ncNewline();
        ncPrint("R9:  0x"); ncPrintHex(registers.r9);  ncNewline();
        ncPrint("R10: 0x"); ncPrintHex(registers.r10); ncNewline();
        ncPrint("R11: 0x"); ncPrintHex(registers.r11); ncNewline();
        ncPrint("R12: 0x"); ncPrintHex(registers.r12); ncNewline();
        ncPrint("R13: 0x"); ncPrintHex(registers.r13); ncNewline();
        ncPrint("R14: 0x"); ncPrintHex(registers.r14); ncNewline();
        ncPrint("R15: 0x"); ncPrintHex(registers.r15); ncNewline();
    } else {
        ncPrint("Error: No se pudieron obtener los registros");
        ncNewline();
    }
    
    ncPrint("========================================");
    ncNewline();
    ncPrint("EXCEPCION MANEJADA - RETORNANDO A SHELL");
    ncNewline();
    ncPrint("El sistema continua funcionando normalmente");
    ncNewline();
    ncPrint("NOTA: La instruccion problematica fue omitida");
    ncNewline();
    ncPrint("========================================");
    ncNewline();
    
    // En lugar de detener el sistema, simplemente retornamos
    // El macro ASM se encargará de ajustar el RIP para evitar repetir la excepción
    return;
}

static void zero_division(uint64_t rip) {
    ncPrint("Tipo: Division por Cero (Exception #0)");
    ncNewline();
    ncPrint("Descripción: Intento de division por cero");
    ncNewline();
    ncPrint("Instruction Pointer (RIP): 0x");
    ncPrintHex(rip);
    ncNewline();
}

static void invalid_opcode(uint64_t rip) {
    ncPrint("Tipo: Codigo de Operacion Invalido (Exception #6)");
    ncNewline();
    ncPrint("Descripcion: Instruccion no reconocida por el CPU");
    ncNewline();
    ncPrint("Instruction Pointer (RIP): 0x");
    ncPrintHex(rip);
    ncNewline();
}