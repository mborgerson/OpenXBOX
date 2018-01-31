#include "common.h"

/*
 * RtlUpcaseUnicodeChar
 *
 * Import Number:      313
 * Calling Convention: stdcall
 * Parameter 0:        WCHAR SourceCharacter
 * Return Type:        WCHAR
 */
int Xbox::RtlUpcaseUnicodeChar()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(WCHAR, SourceCharacter);
	XboxTypes::WCHAR rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
