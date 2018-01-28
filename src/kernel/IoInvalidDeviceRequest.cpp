#include "common.h"

/*
 * IoInvalidDeviceRequest
 *
 * Import Number:      74
 * Calling Convention: stdcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Parameter 1:        PIRP           Irp
 * Return Type:        NTSTATUS
 */
int Xbox::IoInvalidDeviceRequest()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(DEVICE_OBJECT, DeviceObject);
	K_INIT_ARG_PTR(IRP,           Irp);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
