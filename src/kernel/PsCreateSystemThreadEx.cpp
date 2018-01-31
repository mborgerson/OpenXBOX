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
	K_INIT_ARG_VAL(PHANDLE,          ThreadHandle);
	K_INIT_ARG_VAL(SIZE_T,           ThreadExtensionSize);
	K_INIT_ARG_VAL(SIZE_T,           KernelStackSize);
	K_INIT_ARG_VAL(SIZE_T,           TlsDataSize);
	K_INIT_ARG_VAL(PHANDLE,          ThreadId);
	K_INIT_ARG_VAL(PKSTART_ROUTINE,  StartRoutine);
	K_INIT_ARG_VAL(PVOID,            StartContext);
	K_INIT_ARG_VAL(BOOLEAN,          CreateSuspended);
	K_INIT_ARG_VAL(BOOLEAN,          DebuggerThread);
	K_INIT_ARG_VAL(PKSYSTEM_ROUTINE, SystemRoutine);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->PsCreateSystemThreadEx(ThreadHandle, ThreadExtensionSize, KernelStackSize, TlsDataSize, ThreadId, StartRoutine, StartContext, CreateSuspended, DebuggerThread, SystemRoutine);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
