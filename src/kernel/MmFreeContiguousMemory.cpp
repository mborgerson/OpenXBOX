#include "common.h"
#include "memmgr.h"

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
	K_INIT_ARG(XboxTypes::PVOID, BaseAddress);

	if (m_memmgr->Free(BaseAddress)) {
		printf("Free at %x\n", BaseAddress);
	}
	else {
		printf("Attempted to free unallocated memory at %x\n", BaseAddress);
		// TODO: should we stop execution here?
	}

	K_EXIT();
	return 0;
}
