#ifndef TSS_H
#define TSS_H

#include <stdint.h>

// https://wiki.osdev.org/Task_State_Segment

typedef struct _TSS {
	uint16_t link;
	uint16_t _reserved0;
	uint32_t ESP0;
	uint16_t SS0;
	uint16_t _reserved1;
	uint32_t ESP1;
	uint16_t SS1;
	uint16_t _reserved2;
	uint32_t ESP2;
	uint16_t SS2;
	uint16_t _reserved3;
	uint32_t CR3;
	uint32_t EIP;
	uint32_t EFLAGS;
	uint32_t EAX;
	uint32_t ECX;
	uint32_t EDX;
	uint32_t EBX;
	uint32_t ESP;
	uint32_t EBP;
	uint32_t ESI;
	uint32_t EDI;
	uint16_t ES;
	uint16_t _reserved4;
	uint16_t CS;
	uint16_t _reserved5;
	uint16_t SS;
	uint16_t _reserved6;
	uint16_t DS;
	uint16_t _reserved7;
	uint16_t FS;
	uint16_t _reserved8;
	uint16_t GS;
	uint16_t _reserved9;
	uint16_t LDTR;
	uint16_t _reserved10;
	uint16_t _reserved11;
	uint16_t IOPBoffset;

} TSS;

#endif
