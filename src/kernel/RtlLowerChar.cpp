#include "common.h"

/*
 * RtlLowerChar
 *
 * Import Number:      296
 * Calling Convention: stdcall
 * Parameter 0:        CHAR Character
 * Return Type:        CHAR
 */
int Xbox::RtlLowerChar()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(CHAR, Character);
	XboxTypes::CHAR rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
