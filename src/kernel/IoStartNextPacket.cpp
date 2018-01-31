#include "common.h"

/*
 * IoStartNextPacket
 *
 * Import Number:      81
 * Calling Convention: stdcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Return Type:        VOID
 */
int Xbox::IoStartNextPacket()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PDEVICE_OBJECT, DeviceObject);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
