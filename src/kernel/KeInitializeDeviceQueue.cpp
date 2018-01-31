#include "common.h"

/*
 * KeInitializeDeviceQueue
 *
 * Import Number:      106
 * Calling Convention: stdcall
 * Parameter 0:        PKDEVICE_QUEUE DeviceQueue
 * Return Type:        VOID
 */
int Xbox::KeInitializeDeviceQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKDEVICE_QUEUE, DeviceQueue);

	m_kernel->KeInitializeDeviceQueue(DeviceQueue);

	K_EXIT();
	return KF_OK;
}
