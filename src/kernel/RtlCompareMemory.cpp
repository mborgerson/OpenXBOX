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
	K_INIT_ARG_VAL(PVOID,  Source1);
	K_INIT_ARG_VAL(PVOID,  Source2);
	K_INIT_ARG_VAL(SIZE_T, Length);
	XboxTypes::SIZE_T rval;

	rval = m_kernel->RtlCompareMemory(Source1, Source2, Length);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
