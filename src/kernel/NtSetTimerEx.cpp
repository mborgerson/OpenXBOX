#include "common.h"

/*
 * NtSetTimerEx
 *
 * Import Number:      229
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE             TimerHandle
 * Parameter 1:        PLARGE_INTEGER     DueTime
 * Parameter 2:        PTIMER_APC_ROUTINE TimerApcRoutine
 * Parameter 3:        KPROCESSOR_MODE    ApcMode
 * Parameter 4:        PVOID              TimerContext
 * Parameter 5:        BOOLEAN            ResumeTimer
 * Parameter 6:        LONG               Period
 * Parameter 7:        PBOOLEAN           PreviousState
 * Return Type:        NTSTATUS
 */
int Xbox::NtSetTimerEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,             TimerHandle);
	K_INIT_ARG_PTR(LARGE_INTEGER,      DueTime);
	K_INIT_ARG_VAL(PTIMER_APC_ROUTINE, TimerApcRoutine);
	K_INIT_ARG_VAL(KPROCESSOR_MODE,    ApcMode);
	K_INIT_ARG_PTR(VOID,               TimerContext);
	K_INIT_ARG_VAL(BOOLEAN,            ResumeTimer);
	K_INIT_ARG_VAL(LONG,               Period);
	K_INIT_ARG_PTR(BOOLEAN,            PreviousState);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
