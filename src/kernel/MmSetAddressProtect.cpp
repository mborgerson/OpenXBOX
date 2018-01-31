#include "common.h"

/*
 * MmSetAddressProtect
 *
 * Import Number:      182
 * Calling Convention: stdcall
 * Parameter 0:        PVOID BaseAddress
 * Parameter 1:        ULONG NumberOfBytes
 * Parameter 2:        ULONG NewProtect
 * Return Type:        VOID
 */
int Xbox::MmSetAddressProtect()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, BaseAddress);
	K_INIT_ARG(ULONG, NumberOfBytes);
	K_INIT_ARG(ULONG, NewProtect);

	m_kernel->MmSetAddressProtect(BaseAddress, NumberOfBytes, NewProtect);

	K_EXIT();
	return KF_WARN_FAKE_IMPL;
}
