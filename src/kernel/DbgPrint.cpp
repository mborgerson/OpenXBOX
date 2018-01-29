#include "common.h"

/*
 * DbgPrint
 *
 * Import Number:      8
 * Calling Convention: cdecl
 * Parameter 0:        PCH Format
 * VarArgs
 * Return Type:        ULONG
 */
int Xbox::DbgPrint()
{
	K_ENTER_CDECL();
	K_INIT_ARG_PTR(CH, Format);
	XboxTypes::ULONG rval;
	// FIXME: VarArgs

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
