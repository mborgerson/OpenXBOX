#include "common.h"

/*
 * RtlCompareMemory
 *
 * Import Number:      268
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  Source1
 * Parameter 1:        PVOID  Source2
 * Parameter 2:        SIZE_T Length
 * Return Type:        SIZE_T
 */
int Xbox::RtlCompareMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,  Source1);
	K_INIT_ARG_PTR(VOID,  Source2);
	K_INIT_ARG_VAL(SIZE_T, Length);
	XboxTypes::SIZE_T rval;

	rval = Length;
	uint8_t *pBytes1 = (uint8_t *)pSource1;
	uint8_t *pBytes2 = (uint8_t *)pSource2;
	for (uint32_t i = 0; i < Length; i++) {
		if (pBytes1[i] != pBytes2[i]) {
			rval = i;
			break;
		}
	}

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
