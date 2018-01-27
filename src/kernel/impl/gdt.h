#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct _GDTEntry {
	union {
		struct {
			uint16_t limitLow : 16;
			uint16_t baseLow : 16;
			uint8_t baseMid : 8;
			uint8_t access : 8;
			uint8_t limitHigh : 4;
			uint8_t flags : 4;
			uint8_t baseHigh : 8;
		} data;
		uint64_t descriptor;
	};

	void Set(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
} GDTEntry;

#endif
