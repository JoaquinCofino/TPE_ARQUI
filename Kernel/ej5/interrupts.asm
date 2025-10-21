; este archivo es un handler de interrupción para 64 bits

section .text
;global _irq00Handler
global _irq01Handler
extern irqDispatcher

;-----------------------------------------
; Macros para guardar/restaurar registros
;-----------------------------------------
%macro pushState 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popState 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

;-----------------------------------------
; Macro principal de handler de interrupción
;-----------------------------------------
%macro irqHandlerMaster 1
    pushState
    mov rdi, %1
    call irqDispatcher

    cmp rdi, 8
    jb %%irq_maestro_%1        ; si IRQ < 8, saltar a maestro
    ; IRQ >= 8 -> esclavo
    mov al, 0x20
    out 0xA0, al               ; EOI esclavo
    mov al, 0x20
    out 0x20, al               ; EOI maestro
    jmp %%fin_eoi_%1

%%irq_maestro_%1:
    mov al, 0x20
    out 0x20, al               ; EOI maestro

%%fin_eoi_%1:
    popState
    iretq                        ; return from interrupt
%endmacro

;-----------------------------------------
; Handlers concretos
;-----------------------------------------
_irq00Handler: 
    irqHandlerMaster 0          ; Timer IRQ0

_irq01Handler:
    irqHandlerMaster 1          ; Teclado IRQ1
