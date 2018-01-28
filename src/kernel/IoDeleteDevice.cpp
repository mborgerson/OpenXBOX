#include "common.h"

/*
 * IoDeleteDevice
 *
 * Import Number:      68
 * Calling Convention: stdcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Return Type:        VOID
 */
int Xbox::IoDeleteDevice()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(DEVICE_OBJECT, DeviceObject);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
