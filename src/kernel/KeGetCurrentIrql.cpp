#include "common.h"

/*
 * KeGetCurrentIrql
 *
 * Import Number:      103
 * Calling Convention: stdcall
 * Return Type:        KIRQL
 */
int Xbox::KeGetCurrentIrql()
{
	K_ENTER_STDCALL();
	XboxTypes::KIRQL rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
