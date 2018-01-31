#include "common.h"

/*
 * RtlInitAnsiString
 *
 * Import Number:      289
 * Calling Convention: stdcall
 * Parameter 0:        PANSI_STRING DestinationString
 * Parameter 1:        PCSZ         SourceString
 * Return Type:        VOID
 */
int Xbox::RtlInitAnsiString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PANSI_STRING, DestinationString);
	K_INIT_ARG(PCSZ,         SourceString);

	m_kernel->RtlInitAnsiString(DestinationString, SourceString);

	K_EXIT();
	return KF_OK;
}
