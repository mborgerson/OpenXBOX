#include "common.h"

/*
 * RtlFillMemoryUlong
 *
 * Import Number:      285
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  Destination
 * Parameter 1:        SIZE_T Length
 * Parameter 2:        ULONG  Pattern
 * Return Type:        VOID
 */
int Xbox::RtlFillMemoryUlong()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,   Destination);
	K_INIT_ARG_VAL(SIZE_T, Length);
	K_INIT_ARG_VAL(ULONG,  Pattern);

	// Fill 32 bits at a time
	// Any extra bytes are ignored
	uint32_t numDwords = Length >> 2;
	uint32_t *lastAddr = (uint32_t *)pDestination + numDwords;
	for (uint32_t *p = (uint32_t *)pDestination; p < lastAddr; p++) {
		*p = Pattern;
	}

	K_EXIT();
	return KF_OK;
}
