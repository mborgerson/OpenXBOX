#include "common.h"

/*
 * RtlCompareMemoryUlong
 *
 * Import Number:      269
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  Source
 * Parameter 1:        SIZE_T Length
 * Parameter 2:        ULONG  Pattern
 * Return Type:        SIZE_T
 */
int Xbox::RtlCompareMemoryUlong()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,   Source);
	K_INIT_ARG_VAL(SIZE_T, Length);
	K_INIT_ARG_VAL(ULONG,  Pattern);
	XboxTypes::SIZE_T rval;

	// Compare 32 bits at a time
	// Any extra bytes are ignored
	uint32_t numDwords = Length >> 2;
	uint32_t *pDwords = (uint32_t *)pSource;
	rval = Length;
	for (uint32_t i = 0; i < numDwords; i++) {
		if (pDwords[i] != Pattern) {
			rval = i;
			break;
		}
	}

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
