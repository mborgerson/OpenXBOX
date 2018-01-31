#include "common.h"

/*
 * IofCompleteRequest
 *
 * Import Number:      87
 * Calling Convention: fastcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Return Type:        VOID
 */
int Xbox::IofCompleteRequest()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PDEVICE_OBJECT, DeviceObject);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
