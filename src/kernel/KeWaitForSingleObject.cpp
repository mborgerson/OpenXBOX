#include "common.h"

/*
 * KeWaitForSingleObject
 *
 * Import Number:      159
 * Calling Convention: stdcall
 * Parameter 0:        PVOID           Object
 * Parameter 1:        KWAIT_REASON    WaitReason
 * Parameter 2:        KPROCESSOR_MODE WaitMode
 * Parameter 3:        BOOLEAN         Alertable
 * Parameter 4:        PLARGE_INTEGER  Timeout
 * Return Type:        NTSTATUS
 */
int Xbox::KeWaitForSingleObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,            Object);
	K_INIT_ARG_VAL(KWAIT_REASON,    WaitReason);
	K_INIT_ARG_VAL(KPROCESSOR_MODE, WaitMode);
	K_INIT_ARG_VAL(BOOLEAN,         Alertable);
	K_INIT_ARG_PTR(LARGE_INTEGER,   Timeout);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
