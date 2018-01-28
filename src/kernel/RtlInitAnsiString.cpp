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
	K_INIT_ARG_PTR(ANSI_STRING, DestinationString);
	K_INIT_ARG_PTR(CSZ,         SourceString);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
