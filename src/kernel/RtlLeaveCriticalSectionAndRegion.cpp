#include "common.h"

/*
 * RtlLeaveCriticalSectionAndRegion
 *
 * Import Number:      295
 * Calling Convention: stdcall
 * Parameter 0:        PRTL_CRITICAL_SECTION CriticalSection
 * Return Type:        VOID
 */
int Xbox::RtlLeaveCriticalSectionAndRegion()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(RTL_CRITICAL_SECTION, CriticalSection);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
