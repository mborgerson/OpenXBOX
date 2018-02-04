#include "common.h"

/*
 * KeEnterCriticalRegion
 *
 * Import Number:      101
 * Calling Convention: stdcall
 * Return Type:        VOID
 */
int Xbox::KeEnterCriticalRegion()
{
	K_ENTER_STDCALL();

	m_kernel->KeEnterCriticalRegion();

	K_EXIT();
	return KF_OK;
}
