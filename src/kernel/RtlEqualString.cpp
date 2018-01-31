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
	K_INIT_ARG(PSTRING, String1);
	K_INIT_ARG(PSTRING, String2);
	K_INIT_ARG(BOOLEAN, CaseInSensitive);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->RtlEqualString(String1, String2, CaseInSensitive);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
