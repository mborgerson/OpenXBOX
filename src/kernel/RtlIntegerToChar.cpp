#include "common.h"

/*
 * RtlIntegerToChar
 *
 * Import Number:      292
 * Calling Convention: stdcall
 * Parameter 0:        ULONG Value
 * Parameter 1:        ULONG Base
 * Parameter 2:        LONG  OutputLength
 * Parameter 3:        PSZ   String
 * Return Type:        NTSTATUS
 */
int Xbox::RtlIntegerToChar()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(ULONG, Value);
	K_INIT_ARG(ULONG, Base);
	K_INIT_ARG(LONG,  OutputLength);
	K_INIT_ARG(PSZ,   String);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
