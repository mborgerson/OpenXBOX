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
	K_INIT_ARG(PUNICODE_STRING, DestinationString);
	K_INIT_ARG(PUNICODE_STRING, SourceString);

	m_kernel->RtlCopyUnicodeString(DestinationString, SourceString);

	K_EXIT();
	return KF_OK;
}
