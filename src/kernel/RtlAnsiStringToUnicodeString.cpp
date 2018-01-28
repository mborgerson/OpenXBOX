#include "common.h"

/*
 * RtlAnsiStringToUnicodeString
 *
 * Import Number:      260
 * Calling Convention: stdcall
 * Parameter 0:        PUNICODE_STRING DestinationString
 * Parameter 1:        PSTRING         SourceString
 * Parameter 2:        BOOLEAN         AllocateDestinationString
 * Return Type:        NTSTATUS
 */
int Xbox::RtlAnsiStringToUnicodeString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UNICODE_STRING, DestinationString);
	K_INIT_ARG_PTR(STRING,         SourceString);
	K_INIT_ARG_VAL(BOOLEAN,        AllocateDestinationString);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
