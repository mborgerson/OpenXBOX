#include "common.h"

/*
 * PsCreateSystemThreadEx
 *
 * Import Number:      255
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE          ThreadHandle
 * Parameter 1:        SIZE_T           ThreadExtensionSize
 * Parameter 2:        SIZE_T           KernelStackSize
 * Parameter 3:        SIZE_T           TlsDataSize
 * Parameter 4:        PHANDLE          ThreadId
 * Parameter 5:        PKSTART_ROUTINE  StartRoutine
 * Parameter 6:        PVOID            StartContext
 * Parameter 7:        BOOLEAN          CreateSuspended
 * Parameter 8:        BOOLEAN          DebuggerThread
 * Parameter 9:        PKSYSTEM_ROUTINE SystemRoutine
 * Return Type:        NTSTATUS
 */
int Xbox::PsCreateSystemThreadEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HANDLE,           ThreadHandle);
	K_INIT_ARG_VAL(SIZE_T,           ThreadExtensionSize);
	K_INIT_ARG_VAL(SIZE_T,           KernelStackSize);
	K_INIT_ARG_VAL(SIZE_T,           TlsDataSize);
	K_INIT_ARG_PTR(HANDLE,           ThreadId);
	K_INIT_ARG_VAL(PKSTART_ROUTINE,  StartRoutine);
	K_INIT_ARG_PTR(VOID,             StartContext);
	K_INIT_ARG_VAL(BOOLEAN,          CreateSuspended);
	K_INIT_ARG_VAL(BOOLEAN,          DebuggerThread);
	K_INIT_ARG_VAL(PKSYSTEM_ROUTINE, SystemRoutine);
	XboxTypes::NTSTATUS rval;

	// TODO: this is obviously incomplete
	m_kernel->m_sched->ScheduleThread(m_kernel->CreateThread(StartRoutine, KernelStackSize));
	// TODO: use an object manager to create the thread and obtain the handle
	*pThreadHandle = 0x4;

	rval = 0;

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
