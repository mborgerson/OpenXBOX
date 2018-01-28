#include "common.h"

/*
 * RtlEqualString
 *
 * Import Number:      279
 * Calling Convention: stdcall
 * Parameter 0:        PSTRING String1
 * Parameter 1:        PSTRING String2
 * Parameter 2:        BOOLEAN CaseInSensitive
 * Return Type:        BOOLEAN
 */
int Xbox::RtlEqualString()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(STRING,  String1);
	K_INIT_ARG_PTR(STRING,  String2);
	K_INIT_ARG_VAL(BOOLEAN, CaseInSensitive);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
