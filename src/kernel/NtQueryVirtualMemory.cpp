#include "common.h"

/*
 * NtQueryVirtualMemory
 *
 * Import Number:      217
 * Calling Convention: stdcall
 * Parameter 0:        PVOID                     BaseAddress
 * Parameter 1:        PMEMORY_BASIC_INFORMATION MemoryInformation
 * Return Type:        NTSTATUS
 */
int Xbox::NtQueryVirtualMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,                     BaseAddress);
	K_INIT_ARG_PTR(MEMORY_BASIC_INFORMATION, MemoryInformation);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
