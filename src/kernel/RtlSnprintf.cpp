#include "common.h"

/*
 * RtlSnprintf
 *
 * Import Number:      361
 * Calling Convention: cdecl
 * Parameter 0:        PCHAR  pszDest
 * Parameter 1:        SIZE_T cbDest
 * Parameter 2:        LPCSTR pszFormat
 * VarArgs
 * Return Type:        VOID
 */
int Xbox::RtlSnprintf()
{
	K_ENTER_CDECL();
	K_INIT_ARG(PCHAR,  pszDest);
	K_INIT_ARG(SIZE_T, cbDest);
	K_INIT_ARG(LPCSTR, pszFormat);
	// FIXME: VarArgs

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
