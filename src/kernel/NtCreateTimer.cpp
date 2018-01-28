#include "common.h"

/*
 * NtCreateTimer
 *
 * Import Number:      194
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE            TimerHandle
 * Parameter 1:        POBJECT_ATTRIBUTES ObjectAttributes
 * Parameter 2:        TIMER_TYPE         TimerType
 * Return Type:        NTSTATUS
 */
int Xbox::NtCreateTimer()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HANDLE,            TimerHandle);
	K_INIT_ARG_PTR(OBJECT_ATTRIBUTES, ObjectAttributes);
	K_INIT_ARG_VAL(TIMER_TYPE,        TimerType);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
