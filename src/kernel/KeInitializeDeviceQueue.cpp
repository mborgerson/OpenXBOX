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
	K_INIT_ARG_PTR(KDEVICE_QUEUE, DeviceQueue);

	// FIXME: let the object manager initialize this
	pDeviceQueue->Size = sizeof(XboxTypes::KDEVICE_QUEUE);
	pDeviceQueue->Type = XboxTypes::DeviceQueueObject;
	pDeviceQueue->Busy = FALSE;
	InitializeListHead(&pDeviceQueue->DeviceListHead);

	K_EXIT();
	return KF_OK;
}
