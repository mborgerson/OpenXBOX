#include "common.h"

/*
 * RtlDowncaseUnicodeString
 *
 * Import Number:      276
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING DestinationString
 * Parameter 1:        PUNICODE_STRING SourceString
 * Parameter 2:        BOOLEAN         AllocateDestinationString
 * Return Type:        NTSTATUS
 */
int Xbox::RtlDowncaseUnicodeString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUNICODE_STRING, DestinationString);
	K_INIT_ARG(PUNICODE_STRING, SourceString);
	K_INIT_ARG(BOOLEAN,         AllocateDestinationString);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->RtlDowncaseUnicodeString(DestinationString, SourceString, AllocateDestinationString);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
