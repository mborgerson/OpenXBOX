#include "common.h"

/*
 * IofCallDriver
 *
 * Import Number:      86
 * Calling Convention: fastcall
 * Parameter 0:        PDEVICE_OBJECT DeviceObject
 * Parameter 1:        PIRP           Irp
 * Return Type:        NTSTATUS
 */
int Xbox::IofCallDriver()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PDEVICE_OBJECT, DeviceObject);
	K_INIT_ARG(PIRP,           Irp);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
