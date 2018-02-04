#include "common.h"

/*
 * KeRemoveEntryDeviceQueue
 *
 * Import Number:      135
 * Calling Convention: stdcall
 * Parameter 0:        PKDEVICE_QUEUE       DeviceQueue
 * Parameter 1:        PKDEVICE_QUEUE_ENTRY DeviceQueueEntry
 * Return Type:        BOOLEAN
 */
int Xbox::KeRemoveEntryDeviceQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKDEVICE_QUEUE, DeviceQueue);
	K_INIT_ARG(PKDEVICE_QUEUE_ENTRY, DeviceQueueEntry);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeRemoveEntryDeviceQueue(DeviceQueue, DeviceQueueEntry);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
