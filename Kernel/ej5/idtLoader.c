
//archivo que arma la IDT

#include <stdint.h>
extern void _cli();     //cli → Clear Interrupt Flag → deshabilita interrupciones.
extern void _sti();     //sti → Set Interrupt Flag → habilita interrupciones.

extern void picMasterMask(uint8_t mask);        //escribe en el pic maestro
extern void picSlaveMask(uint8_t mask);         //escribe en el pic esclavo
extern void _irq00Handler();                    // handler de IRQ0 en AS. se invoca cada vez que se dispara el timer
extern void _irq01Handler();					//handler de IRQ1

//#pragma pack controlo el alineamiento del guardado de los datos en memoria
#pragma pack(push)			//se guarda la forma de alineamiento actual
#pragma pack (1)			//pido que cada campo se almacene a 1 byte, sin agregar padding
void setUp_IDT_entry(int index, uint64_t offset);
void load_idt();

typedef struct {		
	uint16_t offset_l;	// bits 0..15 de la dirección del handler
	uint16_t selector;	//segmento de código
	uint8_t cero;		//reservado
	uint8_t access;		//
	uint16_t offset_m;	//bits 16..31 de la dirección del handler
	uint32_t offset_h;	//bits 32..63 de la dirección del handler
	uint32_t other_cero;//
} DESCR_INT;

DESCR_INT * idt = (DESCR_INT *) 0;		//puntero a la idt. empieza en la direccion 0
#define ACS_INT 0x8E                    // 10001110b -> Present=1, DPL=00, Type=1110 (interrupt gate 64 bits)


void setUp_IDT_entry(int index, uint64_t offset){		//funcion que recibe el código de la interrupción
	idt[index].offset_l = offset & 0xFFFF;				//y un puntero a su rutina (ubicada en la idt)
	idt[index].selector = 0x08;
	idt[index].cero = 0;
	idt[index].access = ACS_INT;
	idt[index].offset_m = (offset >> 16) & 0xFFFF;
	idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
	idt[index].other_cero = 0;
}

void load_idt() {
	_cli();													//bloquea interrupciones
	setUp_IDT_entry (0x20, (uint64_t) &_irq00Handler);		// Interrupción de timer tick habilitada
	setUp_IDT_entry(0x21, (uint64_t) &_irq01Handler); 		// Interrupción de teclado habilitada
	
	//picMasterMask(0xFE);	//habilita IRQ0
	picMasterMask(0xFC);	//habilita IRQ 0 y 1
	picSlaveMask(0xFF);		//deshabilita todo en esclavo
	_sti(); 				//habilita interrupciones
}
#pragma pack(pop)