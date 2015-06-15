
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

//#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x40;                                                                  \
    idt[numero].attr = (unsigned short) 0x8E00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);



void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);//BOUND Range Exceeded Exception
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);//Double Fault Exception
    IDT_ENTRY(9);//Coprocessor Segment Overrun
    IDT_ENTRY(10);//invalid TSS
    IDT_ENTRY(11);//Segment Not Present
    IDT_ENTRY(12);//Stack Fault Exception
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(17);

    IDT_ENTRY(32); //reloj
    IDT_ENTRY(33); //teclado

    IDT_ENTRY(70); // 0x46    
    idt[70].attr = (unsigned short)0XEE00; //cambio el dpl 
}
