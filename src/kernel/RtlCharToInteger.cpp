#include "common.h"

/*
 * RtlCharToInteger
 *
 * Import Number:      267
 * Calling Convention: stdcall
 * Parameter 0:        PCSZ   String
 * Parameter 1:        ULONG  Base
 * Parameter 2:        PULONG Value
 * Return Type:        NTSTATUS
 */
int Xbox::RtlCharToInteger()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(CSZ,   String);
	K_INIT_ARG_VAL(ULONG, Base);
	K_INIT_ARG_PTR(ULONG, Value);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
