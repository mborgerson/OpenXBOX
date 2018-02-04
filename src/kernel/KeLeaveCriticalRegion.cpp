#include "common.h"

/*
 * KeLeaveCriticalRegion
 *
 * Import Number:      122
 * Calling Convention: stdcall
 * Return Type:        VOID
 */
int Xbox::KeLeaveCriticalRegion()
{
	K_ENTER_STDCALL();

	m_kernel->KeLeaveCriticalRegion();

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
