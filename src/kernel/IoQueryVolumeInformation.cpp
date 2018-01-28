#include "common.h"

/*
 * IoQueryVolumeInformation
 *
 * Import Number:      76
 * Calling Convention: stdcall
 * Parameter 0:        PFILE_OBJECT         FileObject
 * Parameter 1:        FS_INFORMATION_CLASS FsInformationClass
 * Parameter 2:        ULONG                Length
 * Parameter 3:        PVOID                FsInformation
 * Parameter 4:        PULONG               ReturnedLength
 * Return Type:        NTSTATUS
 */
int Xbox::IoQueryVolumeInformation()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(FILE_OBJECT,          FileObject);
	K_INIT_ARG_VAL(FS_INFORMATION_CLASS, FsInformationClass);
	K_INIT_ARG_VAL(ULONG,                Length);
	K_INIT_ARG_PTR(VOID,                 FsInformation);
	K_INIT_ARG_PTR(ULONG,                ReturnedLength);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
