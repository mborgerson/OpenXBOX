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
	K_INIT_ARG_PTR(STRING, DestinationString);
	K_INIT_ARG_PTR(STRING, SourceString);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
