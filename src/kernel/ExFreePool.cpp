#include "common.h"

/*
 * ExFreePool
 *
 * Import Number:      17
 * Calling Convention: stdcall
 * Parameter 0:        PVOID P
 * Return Type:        VOID
 */
int Xbox::ExFreePool()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, P);

	m_kernel->ExFreePool(P);

	K_EXIT();
	return KF_WARN_FAKE_IMPL;
}
