#include "common.h"

/*
 * IoSynchronousFsdRequest
 *
 * Import Number:      85
 * Calling Convention: stdcall
 * Parameter 0:        ULONG          MajorFunction
 * Parameter 1:        PDEVICE_OBJECT DeviceObject
 * Parameter 2:        PVOID          Buffer
 * Parameter 3:        ULONG          Length
 * Parameter 4:        PLARGE_INTEGER StartingOffset
 * Return Type:        NTSTATUS
 */
int Xbox::IoSynchronousFsdRequest()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG,         MajorFunction);
	K_INIT_ARG_PTR(DEVICE_OBJECT, DeviceObject);
	K_INIT_ARG_PTR(VOID,          Buffer);
	K_INIT_ARG_VAL(ULONG,         Length);
	K_INIT_ARG_PTR(LARGE_INTEGER, StartingOffset);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
