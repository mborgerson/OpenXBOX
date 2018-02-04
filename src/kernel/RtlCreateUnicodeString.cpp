#include "common.h"

/*
 * RtlCreateUnicodeString
 *
 * Import Number:      274
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING DestinationString
 * Parameter 1:        PCWSTR          SourceString
 * Return Type:        BOOLEAN
 */
int Xbox::RtlCreateUnicodeString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUNICODE_STRING, DestinationString);
	K_INIT_ARG(PCWSTR,          SourceString);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->RtlCreateUnicodeString(DestinationString, SourceString);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
