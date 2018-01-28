#include "common.h"

/*
 * IoStartPacket
 *
 * Import Number:      83
 * Calling Convention: stdcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Parameter 1:        PIRP           Irp
 * Parameter 2:        PULONG         Key
 * Return Type:        VOID
 */
int Xbox::IoStartPacket()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(DEVICE_OBJECT, DeviceObject);
	K_INIT_ARG_PTR(IRP,           Irp);
	K_INIT_ARG_PTR(ULONG,         Key);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
