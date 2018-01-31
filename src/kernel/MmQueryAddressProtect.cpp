#include "common.h"

/*
 * MmQueryAddressProtect
 *
 * Import Number:      179
 * Calling Convention: stdcall
 * Parameter 0:        PVOID VirtualAddress
 * Return Type:        ULONG
 */
int Xbox::MmQueryAddressProtect()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, VirtualAddress);
	XboxTypes::ULONG rval;

	rval = m_kernel->MmQueryAddressProtect(VirtualAddress);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
