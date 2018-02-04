#include "common.h"

/*
 * RtlCopyString
 *
 * Import Number:      272
 * Calling Convention: stdcall
 * Parameter 0:        PSTRING DestinationString
 * Parameter 1:        PSTRING SourceString
 * Return Type:        VOID
 */
int Xbox::RtlCopyString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PSTRING, DestinationString);
	K_INIT_ARG(PSTRING, SourceString);

	m_kernel->RtlCopyString(DestinationString, SourceString);

	K_EXIT();
	return KF_OK;
}
