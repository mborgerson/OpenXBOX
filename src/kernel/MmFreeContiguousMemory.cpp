#include "common.h"

/*
 * MmFreeContiguousMemory
 *
 * Import Number:      171
 * Calling Convention: stdcall
 * Parameter 0:        PVOID BaseAddress
 * Return Type:        VOID
 */
int Xbox::MmFreeContiguousMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PVOID, BaseAddress);

	m_kernel->MmFreeContiguousMemory(BaseAddress);

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
