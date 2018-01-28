#include "common.h"

/*
 * RtlVsnprintf
 *
 * Import Number:      363
 * Calling Convention: cdecl
 * Parameter 0:        PCHAR  pszDest
 * Parameter 1:        SIZE_T cbDest
 * Parameter 2:        LPCSTR pszFormat
 * Return Type:        VOID
 */
int Xbox::RtlVsnprintf()
{
	K_ENTER_CDECL();
	K_INIT_ARG_PTR(CHAR,   pszDest);
	K_INIT_ARG_VAL(SIZE_T, cbDest);
	K_INIT_ARG_LPT(CSTR,   pszFormat);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
