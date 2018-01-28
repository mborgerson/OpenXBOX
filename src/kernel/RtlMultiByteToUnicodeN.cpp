#include "common.h"

/*
 * RtlMultiByteToUnicodeN
 *
 * Import Number:      299
 * Calling Convention: stdcall
 * Parameter 0:        PWSTR  UnicodeString
 * Parameter 1:        ULONG  MaxBytesInUnicodeString
 * Parameter 2:        PULONG BytesInUnicodeString
 * Parameter 3:        PCHAR  MultiByteString
 * Parameter 4:        ULONG  BytesInMultiByteString
 * Return Type:        NTSTATUS
 */
int Xbox::RtlMultiByteToUnicodeN()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(WSTR,  UnicodeString);
	K_INIT_ARG_VAL(ULONG, MaxBytesInUnicodeString);
	K_INIT_ARG_PTR(ULONG, BytesInUnicodeString);
	K_INIT_ARG_PTR(CHAR,  MultiByteString);
	K_INIT_ARG_VAL(ULONG, BytesInMultiByteString);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
