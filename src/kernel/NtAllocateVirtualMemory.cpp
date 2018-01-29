#include "common.h"

/*
 * NtAllocateVirtualMemory
 *
 * Import Number:      184
 * Calling Convention: stdcall
 * Parameter 0:        PPVOID    BaseAddress
 * Parameter 1:        ULONG_PTR ZeroBits
 * Parameter 2:        PSIZE_T   RegionSize
 * Parameter 3:        ULONG     AllocationType
 * Parameter 4:        ULONG     Protect
 * Return Type:        NTSTATUS
 */
int Xbox::NtAllocateVirtualMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(PVOID,     BaseAddress);
	K_INIT_ARG_VAL(ULONG_PTR, ZeroBits);
	K_INIT_ARG_PTR(SIZE_T,    RegionSize);
	K_INIT_ARG_VAL(ULONG,     AllocationType);
	K_INIT_ARG_VAL(ULONG,     Protect);
	XboxTypes::NTSTATUS rval;

	// TODO: need a virtual memory manager on top of the existing memory manager
	PhysicalMemoryBlock *block = m_pmemmgr->AllocateContiguous(*pRegionSize);
	if (nullptr != block) {
		*pBaseAddress = block->BaseAddress();
		*pRegionSize = block->TotalSize();
		rval = 0;
	}
	else {
		rval = STATUS_NO_MEMORY;
	}

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
