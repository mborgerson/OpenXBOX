#include "common.h"

/*
 * NtFsControlFile
 *
 * Import Number:      200
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE           FileHandle
 * Parameter 1:        HANDLE           Event
 * Parameter 2:        PIO_APC_ROUTINE  ApcRoutine
 * Parameter 3:        PVOID            ApcContext
 * Parameter 4:        PIO_STATUS_BLOCK IoStatusBlock
 * Parameter 5:        ULONG            FsControlCode
 * Parameter 6:        PVOID            InputBuffer
 * Parameter 7:        ULONG            InputBufferLength
 * Parameter 8:        PVOID            OutputBuffer
 * Parameter 9:        ULONG            OutputBufferLength
 * Return Type:        NTSTATUS
 */
int Xbox::NtFsControlFile()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,          FileHandle);
	K_INIT_ARG_VAL(HANDLE,          Event);
	K_INIT_ARG_VAL(PIO_APC_ROUTINE, ApcRoutine);
	K_INIT_ARG_PTR(VOID,            ApcContext);
	K_INIT_ARG_PTR(IO_STATUS_BLOCK, IoStatusBlock);
	K_INIT_ARG_VAL(ULONG,           FsControlCode);
	K_INIT_ARG_PTR(VOID,            InputBuffer);
	K_INIT_ARG_VAL(ULONG,           InputBufferLength);
	K_INIT_ARG_PTR(VOID,            OutputBuffer);
	K_INIT_ARG_VAL(ULONG,           OutputBufferLength);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
