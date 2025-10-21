
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq06Handler
GLOBAL _irq07Handler
GLOBAL _irq08Handler
GLOBAL _irq09Handler
GLOBAL _irq10Handler
GLOBAL _irq11Handler
GLOBAL _irq12Handler
GLOBAL _irq13Handler
GLOBAL _irq14Handler
GLOBAL _irq15Handler

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher

SECTION .text

;-----------------------------------------
; Macros para guardar/restaurar registros
;-----------------------------------------

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
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
	pop rsi
	pop rdi
	pop rbp
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

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro irqHandlerSlave 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt) - SLAVE primero
	mov al, 20h
	out 0A0h, al ; EOI al PIC esclavo
	out 20h, al  ; EOI al PIC maestro

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;-----------------------------------------
; Handlers concretos
;-----------------------------------------

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Floppy Disk Controller
_irq06Handler:
	irqHandlerMaster 6

;Parallel Port / Spurious
_irq07Handler:
	irqHandlerMaster 7

;-------- PIC ESCLAVO (IRQ8-IRQ15) --------

;Real Time Clock
_irq08Handler:
	irqHandlerSlave 8

;ACPI
_irq09Handler:
	irqHandlerSlave 9

;Available
_irq10Handler:
	irqHandlerSlave 10

;Available
_irq11Handler:
	irqHandlerSlave 11

;PS/2 Mouse
_irq12Handler:
	irqHandlerSlave 12

;FPU Exception
_irq13Handler:
	irqHandlerSlave 13

;Primary IDE
_irq14Handler:
	irqHandlerSlave 14

;Secondary IDE
_irq15Handler:
	irqHandlerSlave 15


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1