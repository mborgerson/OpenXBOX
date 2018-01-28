#include "common.h"

/*
 * ExInterlockedAddLargeInteger
 *
 * Import Number:      19
 * Calling Convention: stdcall
 * Parameter 0:        PLARGE_INTEGER Addend
 * Parameter 1:        LARGE_INTEGER  Increment
 * Return Type:        LARGE_INTEGER
 */
int Xbox::ExInterlockedAddLargeInteger()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(LARGE_INTEGER, Addend);
	K_INIT_ARG_VAL(LARGE_INTEGER, Increment);
	XboxTypes::LARGE_INTEGER rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
