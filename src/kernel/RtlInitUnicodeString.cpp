#include "common.h"

/*
 * RtlInitUnicodeString
 *
 * Import Number:      290
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING DestinationString
 * Parameter 1:        PCWSTR          SourceString
 * Return Type:        VOID
 */
int Xbox::RtlInitUnicodeString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UNICODE_STRING, DestinationString);
	K_INIT_ARG_PTR(CWSTR,          SourceString);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
