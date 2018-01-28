#include "common.h"

/*
 * RtlUpperChar
 *
 * Import Number:      316
 * Calling Convention: stdcall
 * Parameter 0:        CHAR Character
 * Return Type:        CHAR
 */
int Xbox::RtlUpperChar()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(CHAR, Character);
	XboxTypes::CHAR rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
