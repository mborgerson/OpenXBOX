#include "common.h"

/*
 * NtDeviceIoControlFile
 *
 * Import Number:      196
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE           FileHandle
 * Parameter 1:        HANDLE           Event
 * Parameter 2:        PIO_APC_ROUTINE  ApcRoutine
 * Parameter 3:        PVOID            ApcContext
 * Parameter 4:        PIO_STATUS_BLOCK IoStatusBlock
 * Parameter 5:        ULONG            IoControlCode
 * Parameter 6:        PVOID            InputBuffer
 * Parameter 7:        ULONG            InputBufferLength
 * Parameter 8:        PVOID            OutputBuffer
 * Parameter 9:        ULONG            OutputBufferLength
 * Return Type:        NTSTATUS
 */
int Xbox::NtDeviceIoControlFile()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(HANDLE,           FileHandle);
	K_INIT_ARG(HANDLE,           Event);
	K_INIT_ARG(PIO_APC_ROUTINE,  ApcRoutine);
	K_INIT_ARG(PVOID,            ApcContext);
	K_INIT_ARG(PIO_STATUS_BLOCK, IoStatusBlock);
	K_INIT_ARG(ULONG,            IoControlCode);
	K_INIT_ARG(PVOID,            InputBuffer);
	K_INIT_ARG(ULONG,            InputBufferLength);
	K_INIT_ARG(PVOID,            OutputBuffer);
	K_INIT_ARG(ULONG,            OutputBufferLength);
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->NtDeviceIoControlFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, IoControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
