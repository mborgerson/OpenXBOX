#include "common.h"

/*
 * RtlCopyUnicodeString
 *
 * Import Number:      273
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING DestinationString
 * Parameter 1:        PUNICODE_STRING SourceString
 * Return Type:        VOID
 */
int Xbox::RtlCopyUnicodeString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UNICODE_STRING, DestinationString);
	K_INIT_ARG_PTR(UNICODE_STRING, SourceString);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
