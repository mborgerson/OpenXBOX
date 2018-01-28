#include "common.h"

/*
 * KeRestoreFloatingPointState
 *
 * Import Number:      139
 * Calling Convention: stdcall
 * Parameter 0:        PKFLOATING_SAVE FloatSave
 * Return Type:        NTSTATUS
 */
int Xbox::KeRestoreFloatingPointState()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KFLOATING_SAVE, FloatSave);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
