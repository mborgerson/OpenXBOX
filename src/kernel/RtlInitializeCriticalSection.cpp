#include "common.h"

/*
 * RtlInitializeCriticalSection
 *
 * Import Number:      291
 * Calling Convention: stdcall
 * Parameter 0:        PRTL_CRITICAL_SECTION CriticalSection
 * Return Type:        VOID
 */
int Xbox::RtlInitializeCriticalSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRTL_CRITICAL_SECTION, CriticalSection);

	m_kernel->RtlInitializeCriticalSection(CriticalSection);

	K_EXIT();
	return KF_OK;
}
