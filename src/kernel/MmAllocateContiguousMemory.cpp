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

	printf("NumberOfBytes            = %x\n", NumberOfBytes);

	PhysicalMemoryBlock *block = m_kernel->m_pmemmgr->AllocateContiguous(NumberOfBytes);
	if (block != nullptr) {
		rval = block->BaseAddress();
		printf("...allocated at %x\n", rval);
	}
	else {
		rval = 0;
		printf("...failed to allocate\n");
	}

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
