#include "common.h"

/*
 * KeSetDisableBoostThread
 *
 * Import Number:      144
 * Calling Convention: stdcall
 * Parameter 0:        PKTHREAD Thread
 * Parameter 1:        LOGICAL  Disable
 * Return Type:        LOGICAL
 */
int Xbox::KeSetDisableBoostThread()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KTHREAD, Thread);
	K_INIT_ARG_VAL(LOGICAL, Disable);
	XboxTypes::LOGICAL rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
