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
	K_INIT_ARG_PTR(VOID, VirtualAddress);
	XboxTypes::ULONG rval;

	rval = m_kernel->m_pmemmgr->QueryProtect(VirtualAddress);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
