#include "common.h"

/*
 * AvSetSavedDataAddress
 *
 * Import Number:      4
 * Calling Convention: stdcall
 * Parameter 0:        PVOID Address
 * Return Type:        VOID
 */
int Xbox::AvSetSavedDataAddress()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, Address);

	m_kernel->AvSetSavedDataAddress(Address);

	K_EXIT();
	return KF_OK;
}
