#include "common.h"

/*
 * NtQueryDirectoryFile
 *
 * Import Number:      207
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE                 FileHandle
 * Parameter 1:        HANDLE                 Event
 * Parameter 2:        PIO_APC_ROUTINE        ApcRoutine
 * Parameter 3:        PVOID                  ApcContext
 * Parameter 4:        PIO_STATUS_BLOCK       IoStatusBlock
 * Parameter 5:        PVOID                  FileInformation
 * Parameter 6:        ULONG                  Length
 * Parameter 7:        FILE_INFORMATION_CLASS FileInformationClass
 * Parameter 8:        POBJECT_STRING         FileName
 * Parameter 9:        BOOLEAN                RestartScan
 * Return Type:        NTSTATUS
 */
int Xbox::NtQueryDirectoryFile()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,                 FileHandle);
	K_INIT_ARG_VAL(HANDLE,                 Event);
	K_INIT_ARG_VAL(PIO_APC_ROUTINE,        ApcRoutine);
	K_INIT_ARG_PTR(VOID,                   ApcContext);
	K_INIT_ARG_PTR(IO_STATUS_BLOCK,        IoStatusBlock);
	K_INIT_ARG_PTR(VOID,                   FileInformation);
	K_INIT_ARG_VAL(ULONG,                  Length);
	K_INIT_ARG_VAL(FILE_INFORMATION_CLASS, FileInformationClass);
	K_INIT_ARG_PTR(OBJECT_STRING,          FileName);
	K_INIT_ARG_VAL(BOOLEAN,                RestartScan);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
