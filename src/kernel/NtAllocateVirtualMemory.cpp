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
	K_INIT_ARG_VAL(PPVOID,    BaseAddress);
	K_INIT_ARG_VAL(ULONG_PTR, ZeroBits);
	K_INIT_ARG_VAL(PSIZE_T,   RegionSize);
	K_INIT_ARG_VAL(ULONG,     AllocationType);
	K_INIT_ARG_VAL(ULONG,     Protect);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->NtAllocateVirtualMemory(BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
