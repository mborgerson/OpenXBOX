#include "common.h"

/*
 * RtlLeaveCriticalSection
 *
 * Import Number:      294
 * Calling Convention: stdcall
 * Parameter 0:        PRTL_CRITICAL_SECTION CriticalSection
 * Return Type:        VOID
 */
int Xbox::RtlLeaveCriticalSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRTL_CRITICAL_SECTION, CriticalSection);

	m_kernel->RtlLeaveCriticalSection(CriticalSection);

	K_EXIT();
	return KF_WARN_PARTIAL_IMPL;
}
