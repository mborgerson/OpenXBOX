#include "common.h"

/*
 * RtlTryEnterCriticalSection
 *
 * Import Number:      306
 * Calling Convention: stdcall
 * Parameter 0:        PRTL_CRITICAL_SECTION CriticalSection
 * Return Type:        BOOLEAN
 */
int Xbox::RtlTryEnterCriticalSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRTL_CRITICAL_SECTION, CriticalSection);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->RtlTryEnterCriticalSection(CriticalSection);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
