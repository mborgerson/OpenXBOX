#include "kernel/impl/kernel.h"

XboxTypes::NTSTATUS XboxKernel::PsCreateSystemThreadEx(
	XboxTypes::PHANDLE ThreadHandle,
	XboxTypes::SIZE_T ThreadExtensionSize,
	XboxTypes::SIZE_T KernelStackSize,
	XboxTypes::SIZE_T TlsDataSize,
	XboxTypes::PHANDLE ThreadId,
	XboxTypes::PKSTART_ROUTINE StartRoutine,
	XboxTypes::PVOID StartContext,
	XboxTypes::BOOLEAN CreateSuspended,
	XboxTypes::BOOLEAN DebuggerThread,
	XboxTypes::PKSYSTEM_ROUTINE SystemRoutine
) {
	XboxTypes::HANDLE *pThreadHandle = ToPointer<XboxTypes::HANDLE>(ThreadHandle);

	// TODO: this is obviously incomplete
	// TODO: use an object manager to create the thread and obtain the handle
	ScheduleNewThread(StartRoutine, KernelStackSize);
	*pThreadHandle = 0x4;

	return STATUS_SUCCESS;
}