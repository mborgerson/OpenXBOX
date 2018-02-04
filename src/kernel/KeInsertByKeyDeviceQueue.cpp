#include "common.h"

/*
 * KeInsertByKeyDeviceQueue
 *
 * Import Number:      114
 * Calling Convention: stdcall
 * Parameter 0:        PKDEVICE_QUEUE       DeviceQueue
 * Parameter 1:        PKDEVICE_QUEUE_ENTRY DeviceQueueEntry
 * Parameter 2:        ULONG                SortKey
 * Return Type:        BOOLEAN
 */
int Xbox::KeInsertByKeyDeviceQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKDEVICE_QUEUE,       DeviceQueue);
	K_INIT_ARG(PKDEVICE_QUEUE_ENTRY, DeviceQueueEntry);
	K_INIT_ARG(ULONG,                SortKey);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeInsertByKeyDeviceQueue(DeviceQueue, DeviceQueueEntry, SortKey);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
