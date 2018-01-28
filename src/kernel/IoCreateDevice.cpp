#include "common.h"

/*
 * IoCreateDevice
 *
 * Import Number:      65
 * Calling Convention: stdcall
 * Parameter 0:        PDRIVER_OBJECT  DriverObject
 * Parameter 1:        ULONG           DeviceExtensionSize
 * Parameter 2:        POBJECT_STRING  DeviceName
 * Parameter 3:        DEVICE_TYPE     DeviceType
 * Parameter 4:        BOOLEAN         Exclusive
 * Parameter 5:        PPDEVICE_OBJECT DeviceObject
 * Return Type:        NTSTATUS
 */
int Xbox::IoCreateDevice()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(DRIVER_OBJECT,  DriverObject);
	K_INIT_ARG_VAL(ULONG,          DeviceExtensionSize);
	K_INIT_ARG_PTR(OBJECT_STRING,  DeviceName);
	K_INIT_ARG_VAL(DEVICE_TYPE,    DeviceType);
	K_INIT_ARG_VAL(BOOLEAN,        Exclusive);
	K_INIT_ARG_PTR(PDEVICE_OBJECT, DeviceObject);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
