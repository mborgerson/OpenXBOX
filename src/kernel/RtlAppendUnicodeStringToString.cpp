#include "common.h"

/*
 * RtlAppendUnicodeStringToString
 *
 * Import Number:      262
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING Destination
 * Parameter 1:        PUNICODE_STRING Source
 * Return Type:        NTSTATUS
 */
int Xbox::RtlAppendUnicodeStringToString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUNICODE_STRING, Destination);
	K_INIT_ARG(PUNICODE_STRING, Source);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->RtlAppendUnicodeStringToString(Destination, Source);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
