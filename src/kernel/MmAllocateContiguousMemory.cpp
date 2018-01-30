#include "common.h"

/*
 * MmAllocateContiguousMemory
 *
 * Import Number:      165
 * Calling Convention: stdcall
 * Parameter 0:        SIZE_T NumberOfBytes
 * Return Type:        PVOID
 */
int Xbox::MmAllocateContiguousMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(SIZE_T, NumberOfBytes);
	XboxTypes::PVOID rval;

	rval = m_kernel->MmAllocateContiguousMemory(NumberOfBytes);
	
	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
